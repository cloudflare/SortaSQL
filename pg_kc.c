/**

Copyright 2011 CloudFlare, Inc.
@author Ian Pye <ian@cloudflare.com>

Provides a bridge from PostgreSQL to Kyoto Cabinet.

Use at your own risk.

*/

#include <kclangc.h>

#include <postgres.h>
#include <utils/builtins.h>
#include <utils/array.h>
#include <catalog/pg_type.h>
#include <fmgr.h>
#include <funcapi.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#include "c/entry.pb-c.h"
#include "common.h"

#ifdef PG_MODULE_MAGIC
    PG_MODULE_MAGIC;
#endif

// Print debugging notes?
//#define CF_DUBUG 1;
//#define CF_NO_DB_IS_ERR 1;

// General constant defines
static const int MAX_JUMP_KEY_LEN = 256;
static const int MIN_ARGS = 3;
static const int START_VARIABLE_INDEX = 3;
static const int END_VARIABLE_INDEX = 7;
static const int MAX_KC_ROW_ENTRY = 256;
static const int KC_MAX_ROW_SER = 1028;
static const int KC_MAX_RID = 1028;

// Hold the DB and its asc. info.
typedef struct {
    KCDB                        *db;
    char                        *db_path;
    KCCUR                       *cur;
    char                        *jump_key;
    Cloudflare__ZoneTimeBucket  *msg;
    unsigned int                next_map;
    KCDB                        *kv_db;
    KCCUR                       *kv_cur;
} KC_ENTRY;

typedef struct {
    char                        *map_name;
    char                        *start_time;
    char                        *kv_map_name;
} KC_DB_INFO;

// Row type
typedef struct {
    int32                       vl_len_;                /* varlena header (do not touch directly!) */
    char                        *doctype;
    char                        *classification;
    char                        *pop;
    char                        *psource;
    char                        *key;
    int64_t                     value;
    //KC_DB_INFO                  db_info; // @TODO -- should this be set, allowing for working in/out functions?
} KC_ROW;

PG_FUNCTION_INFO_V1(kc_row_in);

// @TODO -- make this happy.
Datum kc_row_in(PG_FUNCTION_ARGS) {
    //char       *kv = PG_GETARG_CSTRING(0);
    KC_ROW     *out;

    size_t size = sizeof(KC_ROW);
    out = (KC_ROW *)palloc(size);
    memset(out, '0', size);
    SET_VARSIZE(out, size);
    out->doctype = (char *)palloc(sizeof(char) * MAX_KC_ROW_ENTRY);
    out->classification = (char *)palloc(sizeof(char) * MAX_KC_ROW_ENTRY);
    out->pop = (char *)palloc(sizeof(char) * MAX_KC_ROW_ENTRY);
    out->psource = (char *)palloc(sizeof(char) * MAX_KC_ROW_ENTRY);
    out->key = (char *)palloc(sizeof(char) * MAX_KC_ROW_ENTRY);

    strncpy(out->key, "lolkey -- fill me in", MAX_KC_ROW_ENTRY);
    out->value = 0;
    
    PG_RETURN_POINTER(out);
}

PG_FUNCTION_INFO_V1(kc_row_out);

Datum kc_row_out(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    char       *result;

    result = (char *) palloc(256);
    snprintf(result, KC_MAX_ROW_SER, "{%s,%s,%s,%s,%s,%ld}", in->classification, in->doctype, 
             in->pop, in->psource, in->key, in->value);
    PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(kc_key);

Datum kc_key(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    text       *tout;

    tout = cstring_to_text(in->key);
    PG_RETURN_TEXT_P(tout);
}

PG_FUNCTION_INFO_V1(kc_val);

Datum kc_val(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    int64_t    result = in->value;
    PG_RETURN_INT64(result);
}

PG_FUNCTION_INFO_V1(kc_doctype);

Datum kc_doctype(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    text       *tout;   

    tout = cstring_to_text(in->doctype);
    PG_RETURN_TEXT_P(tout);
}

PG_FUNCTION_INFO_V1(kc_class);

Datum kc_class(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    text       *tout;   

    tout = cstring_to_text(in->classification);
    PG_RETURN_TEXT_P(tout);
}

PG_FUNCTION_INFO_V1(kc_pop);

Datum kc_pop(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    text       *tout;   

    tout = cstring_to_text(in->pop);
    PG_RETURN_TEXT_P(tout);
}

PG_FUNCTION_INFO_V1(kc_psource);

Datum kc_psource(PG_FUNCTION_ARGS) {
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(0);
    text       *tout;   

    tout = cstring_to_text(in->psource);
    PG_RETURN_TEXT_P(tout);
}

PG_FUNCTION_INFO_V1(kc_sum);

Datum kc_sum(PG_FUNCTION_ARGS) {
    int64_t    total = PG_GETARG_INT64(0);
    KC_ROW     *in = (KC_ROW *) PG_GETARG_POINTER(1);
    int64_t    res = total + in->value;

    PG_RETURN_INT64(res);
}

// Open a DB.
bool open_db (KCDB *db, char *map_type, char *start_time) {
    // Figure out what db to open.
    char db_buffer[512];
    snprintf(db_buffer, 256, "%s/%s/pg_agg_map_%s.%s", BASE_DB_DIR, start_time, map_type, DB_TYPE);
    if (!kcdbopen(db, db_buffer, KCOWRITER)) {
#ifdef CF_NO_DB_IS_ERR
        ereport(ERROR,
                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                 errmsg("Error opening db: \"%s\", \"%s\". Make sure that the map_name is valid.", 
                        db_buffer, kcecodename(kcdbecode(db)))));
#endif
#ifdef CF_DUBUG
        ereport(NOTICE,
                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                 errmsg("Error opening db: \"%s\", \"%s\". Make sure that the map_name is valid.", 
                        db_buffer, kcecodename(kcdbecode(db)))));
#endif
        return false;
    }

#ifdef CF_DUBUG        
    ereport(NOTICE,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("Opened db -- %s.", db_buffer)));
#endif 

    return true;
}

/**
   General idea is to provide an interface into a map_name:doctype:pop:psource key stored tree table.
 */

PG_FUNCTION_INFO_V1(kc_expand);

Datum kc_expand(PG_FUNCTION_ARGS) {

    KC_ENTRY                        *search;
    FuncCallContext                 *funcctx;
    int                             call_cntr;
    char                            *kbuf;
    size_t                          ksiz, vsiz;
    const char                      *cvbuf;
    char                            *kv_kbuf = NULL; 
    size_t                          kv_ksiz;
    int                             done;

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        // Make sure that there are enough args.
        if (PG_NARGS() < MIN_ARGS) {
            ereport(ERROR,
                    (errcode(ERRCODE_EXTERNAL_ROUTINE_INVOCATION_EXCEPTION),
                     errmsg("Must run expand with at least %d args!", MIN_ARGS)));
        }

        /* Make the kcdb here. */
        search = (KC_ENTRY *)palloc(sizeof(KC_ENTRY)); 
        search->db = kcdbnew();
        if (open_db (search->db, text_to_cstring(PG_GETARG_TEXT_PP(0)), text_to_cstring(PG_GETARG_TEXT_PP(1)))) {

            // Set the key to jump into:
            // Call with -- map_name, result_id, class, doctype, pop, psource
            // Here, map_name describes a db to open.
            // Otherwise, result_id:class:doctype:pop:psource
            (search->jump_key) = (char *) palloc(MAX_JUMP_KEY_LEN * sizeof(char));

            int index_point;
            search->jump_key = text_to_cstring(PG_GETARG_TEXT_PP(2));
            int size_left = MAX_JUMP_KEY_LEN;
            for (index_point = START_VARIABLE_INDEX; index_point < END_VARIABLE_INDEX; index_point++) {
                if (PG_NARGS() > index_point) {
                    char *next_idx = text_to_cstring(PG_GETARG_TEXT_PP(index_point));
                    if (next_idx != NULL) {
                        size_left = size_left - (2 + strlen(next_idx));
                        strncat (search->jump_key, CF_LABEL_SEP, size_left);
                        strncat (search->jump_key, next_idx, size_left);
                    }
                }
            }
            
#ifdef CF_DUBUG
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("Setting jump buffer -- [%s]", search->jump_key)));
#endif
            
            // Create a cursor, and set it to the base point looking for entries.
            search->cur = kcdbcursor(search->db);
            kccurjumpkey(search->cur, search->jump_key, MAX_JUMP_KEY_LEN);
        } else {
            search->db = NULL;
        }

        search->next_map = 0;
        search->msg = NULL;
        
        // Save the search struct for the subsequent calls.
        funcctx->user_fctx = search;

        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    search = (KC_ENTRY *) funcctx->user_fctx;
    
    // If no current msg, try to get the next one.
    done = 1;

#ifdef CF_DUBUG
    ereport(NOTICE,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("beginning run")));
#endif

    if (search->msg) {

#ifdef CF_DUBUG  
        ereport(NOTICE,
                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                 errmsg("Incrementing next from map %d -- %zu", search->next_map, search->msg->n_map_entry)));
#endif

        // Case if we are using the external cursor running over kv map.
        // Ready the next 
        if (search->msg->kv_map_file) {
            
            if ((kv_kbuf = kccurgetkey(search->kv_cur, &kv_ksiz, 1)) == NULL) {
                done = 1;
                kccurdel(search->kv_cur);
                kcdbendtran (search->kv_db, 1);            
                if (!kcdbclose(search->kv_db)) {
                    ereport(ERROR,
                            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                             errmsg("Error Closeing db: \"%s\"", kcecodename(kcdbecode(search->kv_db)))));
                }

                // Also need to free this.
                cloudflare__zone_time_bucket__free_unpacked(search->msg, NULL);
                search->msg = NULL;

            } else {
                done = 0;
            }


        } else {
            if (search->next_map >= search->msg->n_map_entry) {
                // Done with this msg -- move on to the next one.
                cloudflare__zone_time_bucket__free_unpacked(search->msg, NULL);
                search->msg = NULL;
            } else {
                done = 0;
            }
        }
    }

    if (search->db && !search->msg) {
      
#ifdef CF_DUBUG  
        ereport(NOTICE,
                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                 errmsg("Getting new buf -- %s", search->jump_key)));
#endif        

        if ((kbuf = kccurget(search->cur, &ksiz, &cvbuf, &vsiz, 1)) != NULL) {
            // Pull up the PB and expand it.
            search->msg = cloudflare__zone_time_bucket__unpack(NULL, vsiz, (const uint8_t *)cvbuf);
            if (search->msg == NULL) {   // Something failed
                ereport(ERROR,
                        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                         errmsg("error unpacking incoming message")));
                done = 1;
            } else {
                // Does the buffer match the searched for string?
                // @TODO -- bound this?
                if (strstr(search->msg->db_key, search->jump_key)) {
                    done = 0;
                    search->next_map = 0;

                    // And load the kvkc if needed.
                    if (search->msg->kv_map_file) {
                        
#ifdef CF_DUBUG  
                        ereport(NOTICE,
                                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                                 errmsg("Switching to kvs %s", search->msg->kv_map_file)));
#endif

                        search->kv_db = kcdbnew();
                        
                        if (!kcdbopen(search->kv_db, search->msg->kv_map_file, KCOWRITER)) {
#ifdef CF_NO_DB_IS_ERR
                            ereport(ERROR,
                                    (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                                     errmsg("Error opening db: \"%s\", \"%s\". Make sure that the map_name is valid.", 
                                            search->msg->kv_map_file, kcecodename(kcdbecode(search->kv_db)))));
#endif
#ifdef CF_DUBUG
                            ereport(NOTICE,
                                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                                     errmsg("Error opening db: \"%s\", \"%s\". Make sure that the map_name is valid.", 
                                            search->msg->kv_map_file, kcecodename(kcdbecode(search->kv_db)))));
#endif
                            done = 1;
                        } else {
                            kcdbbegintran (search->kv_db, 0);
                            search->kv_cur = kcdbcursor(search->kv_db);
                            kccurjump(search->kv_cur);   

                            if ((kv_kbuf = kccurgetkey(search->kv_cur, &kv_ksiz, 1)) == NULL) {
                                done = 1;
                                kccurdel(search->kv_cur);
                                kcdbendtran (search->kv_db, 1);
                                if (!kcdbclose(search->kv_db)) {
                                    ereport(ERROR,
                                            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                                             errmsg("Error Closeing db: \"%s\"", kcecodename(kcdbecode(search->kv_db)))));
                                }
                            } else {
                                done = 0;
                            }
                        }
                    }
                } else {
                    done = 1;
                }
            }
            kcfree(kbuf);
        } else {
#ifdef CF_DUBUG
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("no msg to find")));
#endif
            done = 1;
        }
    }

#ifdef CF_DUBUG
    ereport(NOTICE,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("Done? %d -- next buf -- %d", done, search->next_map)));   
#endif

    // Take the next iteration over the cursor. If the next is NULL or else not matching the resultid passed in
    // End. Otherwise, parse the value, populating the next row of the returning tuple.
    if (!done) {
        KC_ROW                          *out;
        Datum                           result;

        size_t size = sizeof(KC_ROW);
        out = (KC_ROW *)palloc(size);
        memset(out, '0', size);
        SET_VARSIZE(out, size);

        out->classification = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
        out->doctype = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
        out->pop = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
        out->psource = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
        out->key = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));

        strncpy(out->classification, search->msg->classification, MAX_KC_ROW_ENTRY);
        strncpy(out->doctype, search->msg->doctype, MAX_KC_ROW_ENTRY);
        strncpy(out->pop, search->msg->pop, MAX_KC_ROW_ENTRY);
        strncpy(out->psource, search->msg->psource, MAX_KC_ROW_ENTRY);

        if (search->msg->kv_map_file) {

#ifdef CF_DUBUG
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("getting val from -- [%s]", search->msg->kv_map_file)));
#endif

            snprintf(out->key, MAX_KC_ROW_ENTRY, "%s", kv_kbuf);
            out->value = kcdbincrint (search->kv_db, kv_kbuf, kv_ksiz, 0);

            if (out->value == INT64_MIN) {
                ereport(NOTICE,
                        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                         errmsg("ERROR Getting val from key -- [%s], %s", kv_kbuf, kcecodename(kcdbecode(search->kv_db)))));
            }

            kcfree(kv_kbuf);
        } else {

#ifdef CF_DUBUG
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("Loading %s %ld", search->msg->map_entry[search->next_map]->key, 
                            search->msg->map_entry[search->next_map]->value)));
#endif

            snprintf(out->key, MAX_KC_ROW_ENTRY, "%s", search->msg->map_entry[search->next_map]->key);        
            out->value = search->msg->map_entry[search->next_map]->value;
        }

        result = PointerGetDatum(out);

        /* clean up (this is not really necessary) */
        pfree(out->classification);
        pfree(out->doctype);
        pfree(out->pop);
        pfree(out->psource);
        pfree(out->key);
        pfree(out);

        // Remember that we are going to the next step.
        search->next_map++;

        SRF_RETURN_NEXT(funcctx, result);
    } else {    /* do when there is no more left */
        if (search->db) {
            kccurdel(search->cur);
            if (!kcdbclose(search->db)) {
                ereport(ERROR,
                        (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                         errmsg("Error Closeing db: \"%s\"", kcecodename(kcdbecode(search->db)))));
            }
            
            if (search->msg != NULL) {
                cloudflare__zone_time_bucket__free_unpacked(search->msg, NULL);
            }
            
            pfree(search->jump_key);
        }
        pfree(search);

#ifdef CF_DUBUG
        ereport(NOTICE,
                (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                 errmsg("Done with run")));
#endif

        // Don't delete db, this leads to segfaults.
        SRF_RETURN_DONE(funcctx);
    }
}

inline void set_kv_path(Cloudflare__ZoneTimeBucket *msg, char *map_name, char *start_time_uid, KCDB* db) {

    struct stat path_buf;
    char dir_buf[KC_MAX_RID];
    char *found;

    msg->kv_map_file = (char *)palloc(KC_MAX_RID * sizeof(char));
    snprintf(msg->kv_map_file, KC_MAX_RID, "%s/%s/%s/%s.%s", 
             CF_KV_MAP_BASE,
             start_time_uid,
             map_name,
             msg->db_key,
             DB_TYPE);

    snprintf(dir_buf, KC_MAX_RID, "%s", CF_KV_MAP_BASE);        
    if (stat(dir_buf, &path_buf) != 0) {
        mkdir (dir_buf, NEW_DIR_MASK);
    }
    chmod (dir_buf, NEW_DIR_MODE);
    
    found = strtok (start_time_uid, "/");
    while (found != NULL) {
        strncat (dir_buf, "/", KC_MAX_RID);
        strncat (dir_buf, found, KC_MAX_RID);
        if (stat(dir_buf, &path_buf) != 0) {
            mkdir (dir_buf, NEW_DIR_MODE);
        }
        chmod (dir_buf, NEW_DIR_MODE);
        found = strtok (NULL, "/");
    }
    strncat (dir_buf, "/", KC_MAX_RID);
    strncat (dir_buf, map_name, KC_MAX_RID);
    if (stat(dir_buf, &path_buf) != 0) {
        mkdir (dir_buf, NEW_DIR_MODE);
    }
    chmod (dir_buf, NEW_DIR_MODE);

    if (!kcdbopen(db, msg->kv_map_file, KCOWRITER | KCOCREATE)) {
        ereport(ERROR,
                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                 errmsg("KV open error for: %s %s\n", msg->kv_map_file, kcecodename(kcdbecode(db)))));
    }
    kcdbbegintran (db, 0);
    kcdbclear (db);
    chmod (msg->kv_map_file, FILE_MODE);
}

int merge_using_kv_map(Cloudflare__ZoneTimeBucket *msg_old, Cloudflare__ZoneTimeBucket *msg_new, KCDB* db) {

    unsigned int i;
    int num_new = 0;
    if (msg_new->kv_map_file) {
        // Open a db for the new map file.
        KCCUR* cur;
        KCDB* new_db;
        new_db = kcdbnew();
        /* open the database */
        if (!kcdbopen(new_db, msg_new->kv_map_file, KCOWRITER)) {
            ereport(ERROR,
                    (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                     errmsg("open error: %s", kcecodename(kcdbecode(new_db)))));
        }
        
        cur = kcdbcursor(new_db);
        kccurjump(cur);
        
        char *kv_kbuf;
        size_t kv_ksiz;
        while ((kv_kbuf = kccurgetkey(cur, &kv_ksiz, 1)) != NULL) {
            int64_t value = kcdbincrint (new_db, kv_kbuf, kv_ksiz, 0);
            
#ifdef CF_DUBUG
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("Saving: %s -- %ld", kv_kbuf, value)));
#endif

            kcdbincrint (db, kv_kbuf, kv_ksiz, value);
            num_new++;
            kcfree(kv_kbuf);
        }
        kccurdel(cur);
        kcdbclose(new_db);            
    } else {
        for (i = 0; i < msg_new->n_map_entry; i++) {
            kcdbincrint (db, msg_new->map_entry[i]->key, strlen(msg_new->map_entry[i]->key), msg_new->map_entry[i]->value);
            num_new++;
        }
    }

    return num_new;
}

int merge_messages_basic(Cloudflare__ZoneTimeBucket *msg_old, Cloudflare__ZoneTimeBucket *msg_new) {
    // Iterate over the list of the new, adding to the old.
    unsigned int i,j,last_entry;
    int num_new = 0;
    bool found = false;

    last_entry = msg_old->n_map_entry;
    for (i = 0; i < msg_new->n_map_entry; i++) {
        found = false;
        for (j = 0; j < last_entry; j++) {
            if (strcmp(msg_old->map_entry[j]->key, msg_new->map_entry[i]->key) == 0) {
                msg_old->map_entry[j]->value += msg_new->map_entry[i]->value;
                found = true;
                break;
            }
        }

        // If we get here, it is a new key. Copy over the info.
        if (!found) {
            msg_old->map_entry[msg_old->n_map_entry] = palloc(sizeof(Cloudflare__ZoneTimeBucket__Counter));
            memcpy (msg_old->map_entry[msg_old->n_map_entry], msg_new->map_entry[i], sizeof(Cloudflare__ZoneTimeBucket__Counter));
            msg_old->map_entry[msg_old->n_map_entry]->key = palloc(strlen(msg_new->map_entry[i]->key) + 1);
            strcpy(msg_old->map_entry[msg_old->n_map_entry]->key, msg_new->map_entry[i]->key);
            msg_old->n_map_entry++;
            num_new++;
        }
    }
    return num_new;
}

// Now, a function to shrink KC values down. Need to open in write mode for this ...
// map_name, new rid, array of rids, class, doctype, pop, psource
// Idea is to:
// Make a new buffer, with the passed in value.
// Iterate over the list of rids, doing rid:class:doctype:pop:psource. Get buffer.
// Go over hashmap, adding to the buffers map.
// Save using the new buffer info.

PG_FUNCTION_INFO_V1(kc_shrink);

Datum kc_shrink(PG_FUNCTION_ARGS) {
    
    char       *map_name = text_to_cstring(PG_GETARG_TEXT_PP(0));
    char       *start_time = text_to_cstring(PG_GETARG_TEXT_PP(1)); // Start time + uid!!!
    char       *new_rid = text_to_cstring(PG_GETARG_TEXT_PP(2));
    ArrayType  *old_rids = PG_GETARG_ARRAYTYPE_P(3);
    char       *classification = text_to_cstring(PG_GETARG_TEXT_PP(4));
    char       *doctype = text_to_cstring(PG_GETARG_TEXT_PP(5));
    char       *pop = text_to_cstring(PG_GETARG_TEXT_PP(6));
    char       *psource = text_to_cstring(PG_GETARG_TEXT_PP(7));
    text       *tout;   
    int        i,j;
    Datum      *rid_datums;
    bool       *rid_nulls;
    int        rid_count;
    char       *next_rid;
    KCDB       *main_db;
    char       *vbuf;
    size_t      vsiz;

    // Open our DB.
    main_db = kcdbnew();
    if (!open_db (main_db, map_name, start_time)) {
        tout = cstring_to_text(new_rid);
        PG_RETURN_TEXT_P(tout);        
    }
    kcdbbegintran (main_db, 0);
    
    // First fill in what we can from the input.
    Cloudflare__ZoneTimeBucket msg = CLOUDFLARE__ZONE_TIME_BUCKET__INIT;
    
    msg.map_name = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.doctype = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.classification = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.pop = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.psource = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.result_id = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.db_key = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.db_path = (char *)palloc(MAX_KC_ROW_ENTRY * sizeof(char));
    msg.map_entry = palloc(MAX_KEYS_BEFORE_KV_MAP * sizeof(Cloudflare__ZoneTimeBucket__Counter));
    msg.n_map_entry = 0;

    strncpy(msg.map_name, map_name, MAX_KC_ROW_ENTRY);
    strncpy(msg.classification, classification, MAX_KC_ROW_ENTRY);
    strncpy(msg.doctype, doctype, MAX_KC_ROW_ENTRY);
    strncpy(msg.pop, pop, MAX_KC_ROW_ENTRY);
    strncpy(msg.psource, psource, MAX_KC_ROW_ENTRY);
    strncpy(msg.result_id, new_rid, KC_MAX_RID);
    snprintf(msg.db_path, MAX_KC_ROW_ENTRY, "%s%s%s", 
             map_name, "/", start_time);

    snprintf(msg.db_key, KC_MAX_RID, "%s%s%s%s%s%s%s%s%s%s%s", 
             new_rid, CF_LABEL_SEP,
             classification, CF_LABEL_SEP, 
             doctype, CF_LABEL_SEP,
             pop, CF_LABEL_SEP,
             psource, CF_LABEL_SEP,
             map_name);

    // Now run over the array.
    deconstruct_array(old_rids, TEXTOID, -1, false, 'i',
                      &rid_datums, &rid_nulls, &rid_count);
    if (ARR_HASNULL(old_rids)) {
        ereport(ERROR,
                (errcode(ERRCODE_ARRAY_ELEMENT_ERROR),
                 errmsg("cannot work with arrays containing NULLs")));
    }

    int num_new_keys = 0;
    int num_entries = 0;
    char keys_to_use[rid_count][KC_MAX_RID];
    Cloudflare__ZoneTimeBucket *msg_new[rid_count];
    j=0;
    for (i = 0; i < rid_count; i++) {
        next_rid = TextDatumGetCString(rid_datums[i]);
        snprintf(keys_to_use[i], KC_MAX_RID, "%s%s%s%s%s%s%s%s%s%s%s", 
                 next_rid, CF_LABEL_SEP, 
                 classification, CF_LABEL_SEP,
                 doctype, CF_LABEL_SEP,
                 pop, CF_LABEL_SEP,
                 psource, CF_LABEL_SEP,
                 map_name);

        vbuf = kcdbget(main_db, keys_to_use[i], strlen(keys_to_use[i]), &vsiz);
        if (vbuf) {
            msg_new[j] = cloudflare__zone_time_bucket__unpack(NULL, vsiz, (const uint8_t *)vbuf);
            if (msg_new[j] == NULL) {   // Something failed
                ereport(ERROR,
                        (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                         errmsg("error unpacking incoming message")));
            } else {
                if (msg_new[j]->kv_map_file) {
                    num_entries = MAX_KEYS_BEFORE_KV_MAP + 1;                    
                } else {
                    num_entries += msg_new[j]->n_map_entry; 
                }
                j++;
            }
            kcfree(vbuf);
        } else {
#ifdef CF_DUBUG
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("get error on %s -- %s", keys_to_use[i], kcecodename(kcdbecode(main_db)))));
#endif
        }
    }
    
    // Now merge the buffers.
    KCDB* msg_db = NULL;
    if (num_entries > MAX_KEYS_BEFORE_KV_MAP) {
        msg_db = kcdbnew();
        set_kv_path(&msg, map_name, start_time, msg_db);
    }

    for (i = 0; i < j; i++) {
        if (num_entries > MAX_KEYS_BEFORE_KV_MAP) {
            num_new_keys += merge_using_kv_map(&msg, msg_new[i], msg_db);
        } else {
            num_new_keys += merge_messages_basic(&msg, msg_new[i]);
        }
        cloudflare__zone_time_bucket__free_unpacked(msg_new[i], NULL);
    }

    if (num_entries > MAX_KEYS_BEFORE_KV_MAP) {
        // Close the db.
        kcdbendtran (msg_db, 1);
        kcdbclose(msg_db);
    }

#ifdef CF_DUBUG
    ereport(NOTICE,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
             errmsg("saving: num map entries: %zu -- writting with %d keys", msg.n_map_entry, num_new_keys)));
#endif

    // Save the updated buffer.
    if (num_new_keys > 0) {
        unsigned int len;
        void *buf;
        len = cloudflare__zone_time_bucket__get_packed_size (&msg);  
        buf = palloc (len);           
        cloudflare__zone_time_bucket__pack (&msg, buf);
        if(!kcdbset(main_db, msg.db_key, strlen(msg.db_key), buf, len)) {
            ereport(ERROR,
                    (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                     errmsg("set error: %s\n", kcecodename(kcdbecode(main_db)))));
        }
        pfree (buf);
    }

    // Done!
    kcdbendtran (main_db, 1);
    if (!kcdbclose(main_db)) {
        ereport(ERROR,
                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                 errmsg("Error Closeing db: \"%s\"", kcecodename(kcdbecode(main_db)))));
    }

    tout = cstring_to_text(new_rid);
    PG_RETURN_TEXT_P(tout);
}


// Delete all entries in the db for the given map and prefix + any rid passed in.

PG_FUNCTION_INFO_V1(kc_delete);

Datum kc_delete(PG_FUNCTION_ARGS) {
    
    char       *map_name = text_to_cstring(PG_GETARG_TEXT_PP(0));
    char       *start_time = text_to_cstring(PG_GETARG_TEXT_PP(1));
    ArrayType  *rids = PG_GETARG_ARRAYTYPE_P(2);
    int        i;
    Datum      *rid_datums;
    bool       *rid_nulls;
    int        rid_count;
    char       *next_rid;
    KCDB       *main_db;
    char       *vbuf;
    size_t      vsiz;
    int64_t     num_keys_to_run;
    int64_t     num_keys_deleted;
    char        **keys_to_use;
    Cloudflare__ZoneTimeBucket *msg_new;

    // Open our DB.
    main_db = kcdbnew();
    if (!open_db (main_db, map_name, start_time)) {
        PG_RETURN_INT64(0);
    }
    kcdbbegintran (main_db, 0);

    // Now run over the array.
    deconstruct_array(rids, TEXTOID, -1, false, 'i',
                      &rid_datums, &rid_nulls, &rid_count);
    if (ARR_HASNULL(rids)) {
        ereport(ERROR,
                (errcode(ERRCODE_ARRAY_ELEMENT_ERROR),
                 errmsg("cannot work with arrays containing NULLs")));
    }

    keys_to_use = (char **)palloc(KC_MAX_ENTRIES_PER_RID * sizeof(char));
    num_keys_deleted = 0;
    char prefixes_to_use[rid_count][KC_MAX_RID];

    for (i = 0; i < rid_count; i++) {
        next_rid = TextDatumGetCString(rid_datums[i]);
        snprintf(prefixes_to_use[i], KC_MAX_RID, "%s%s", next_rid, CF_LABEL_SEP);
        num_keys_to_run = kcdbmatchprefix (main_db,	prefixes_to_use[i], keys_to_use, KC_MAX_ENTRIES_PER_RID);
        if (num_keys_to_run != -1) {
            num_keys_deleted += num_keys_to_run;
            int next_key;
            for (next_key=0; next_key < num_keys_to_run; next_key++) {
                vbuf = kcdbget(main_db, keys_to_use[next_key], strlen(keys_to_use[next_key]), &vsiz);
                if (vbuf) {
                    msg_new = cloudflare__zone_time_bucket__unpack(NULL, vsiz, (const uint8_t *)vbuf);
                    if (msg_new == NULL) {   // Something failed
                        ereport(ERROR,
                                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                                 errmsg("error unpacking incoming message")));
                    } else {
                        if (msg_new->kv_map_file) {
                            unlink(msg_new->kv_map_file);
                        }
                        kcdbremove (main_db, keys_to_use[next_key], strlen(keys_to_use[next_key]));	
                    }
                    cloudflare__zone_time_bucket__free_unpacked(msg_new, NULL);
                    kcfree(vbuf);
                    kcfree(keys_to_use[next_key]);
                } else {
                    ereport(NOTICE,
                            (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                             errmsg("get error on %s -- %s", keys_to_use[next_key], kcecodename(kcdbecode(main_db)))));
                }
            }
        } else {
            ereport(NOTICE,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("prefix error on %s -- %s", prefixes_to_use[i], kcecodename(kcdbecode(main_db)))));
        }
    }

    pfree(keys_to_use);
    
    // Done!
    kcdbendtran (main_db, 1);
    if (!kcdbclose(main_db)) {
        ereport(ERROR,
                (errcode(ERRCODE_EXTERNAL_ROUTINE_EXCEPTION),
                 errmsg("Error Closeing db: \"%s\"", kcecodename(kcdbecode(main_db)))));
    }

    PG_RETURN_INT64(num_keys_deleted);
}
