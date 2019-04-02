/**
   Goals here:

   Read in a merged sql dump file.
   Write the sql file to postgres.
   Strip out the k/v info, instead writing this info directly into KC.

   @author ian@cloudflare.com
*/

#include <cstdio>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <string>
#include <stdlib.h>
#include <pcrecpp.h>

#include <zlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <kcpolydb.h>
#include <json/json.h>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/tokenizer.hpp>
#include <arpa/inet.h>

#include "cpp/entry.pb.h"
#include "common.h"

using namespace kyotocabinet;
using namespace std;

// Do we want to clear out any cruft from the DB?
//static const uint32_t g_sql_db_flags = PolyDB::OWRITER | PolyDB::OCREATE | PolyDB::OTRUNCATE;
static const uint32_t g_sql_db_flags = PolyDB::OWRITER | PolyDB::OCREATE;

// States
enum merge_state_t { DELETE = 10, AGG = 20, MAP = 40 };    
enum merge_map_t { MAP_RID, MAP_CLASSIFICATION, MAP_DOCTYPE, MAP_POP, MAP_PSOURCE, MAP_NAME, MAP_KEY, MAP_VALUE };    

// And how to tell what state we're in ...
static const pcrecpp::RE CF_STATE_RE("^(BEGIN)|^(COPY)|(\\\\.)|(COMMIT)");

// Pull out the proper start time.
static const char *DEFAULT_UID_DIR = "-1";
static const char *DEFAULT_TIME_DIR = "default_time";
static const pcrecpp::RE CF_START_TIME_RE("\'(.*?) (.*)");
static const pcrecpp::RE CF_START_TIME_DUMP_RE("(.*?) (.*)");
static const pcrecpp::RE CF_RID_RE("^(.*?)\\s");
static const pcrecpp::RE CF_ALL_RE("(.*?)\\s");

// And how fields are separated. 
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
static boost::char_separator<char> CF_MAP_SEP("\t");

// Handle the closing of dbs as we go though.
static const unsigned int MAP_DBS_TO_PRUNE = 200;
static const unsigned int MAX_MAP_SIZE = 400;

static int64_t g_next_db = 0;

// Macros to do simple things.
inline void set_kv_path(cloudflare::ZoneTimeBucket &msg, string &map_db_key_str) {
    string map_dir(CF_KV_MAP_BASE);
    size_t found = map_dir.length();
    struct stat path_buf;
    
    map_dir = map_dir + "/" + msg.db_path();
    found = map_dir.find_first_of("/", found);
    while (found != string::npos) {
        string tmp_found = map_dir.substr(0, found);
        if (stat(tmp_found.c_str(), &path_buf) != 0) {
            mkdir (tmp_found.c_str(), NEW_DIR_MASK);
            chmod (tmp_found.c_str(), NEW_DIR_MODE);
        }
        found = map_dir.find_first_of("/", found + 1);
    }

    // And finish off the last one.
    if (stat(map_dir.c_str(), &path_buf) != 0) {
        mkdir (map_dir.c_str(), NEW_DIR_MASK);
        chmod (map_dir.c_str(), NEW_DIR_MODE);
    }

    msg.set_kv_map_file(map_dir + "/" + map_db_key_str + "." + DB_TYPE);
}

inline void close_kv_db(PolyDB *current_map_kv, bool &db_open) {
    if (db_open) {
        current_map_kv->end_transaction(1);
        if (current_map_kv->close()) {
            db_open = false;
        } else {
            cerr << "close error: " << current_map_kv->error().name() << endl;
        }
    }
}

inline void open_kv_db(PolyDB *current_map_kv, cloudflare::ZoneTimeBucket &msg, bool &db_open) {

    // Do nothing if the db is already open to the right one.
    if (db_open && current_map_kv->path() == msg.kv_map_file()) {
        return;
    }

    // Always close before opening.
    close_kv_db(current_map_kv, db_open);

    if (!db_open) {
        if (current_map_kv->open(msg.kv_map_file(), g_sql_db_flags)) {
            chmod (msg.kv_map_file().c_str(), FILE_MODE);
            db_open = true;
            current_map_kv->begin_transaction();
        } else {
            cerr << msg.kv_map_file() <<  " -- open error: " << current_map_kv->error().name() << endl;
        }
    }
}

inline void prune_map(map<string, PolyDB *> &dbs, PolyDB *dbs2queue, cloudflare::ZoneTimeBucket &msg) {

    // Close all of the agg map dbs.
    DB::Cursor* cur = dbs2queue->cursor();
    cur->jump();
    unsigned int count = 0;
    string value;
    string old_db = msg.db_path();
    while ((count < MAP_DBS_TO_PRUNE) && ((cur->get_value(&value, false)))) {
        if (value.compare(old_db) != 0) {
            dbs[value]->end_transaction(1);
            dbs[value]->close();
            delete dbs[value];
            dbs.erase(value);        
            cur->remove();
        } else { // Make sure that we ALWAYS step the cursor here.
            cur->step();
        }
        count++;
    }
    delete cur;
}

inline void open_main_db (PolyDB *db, string &map_name, string &time_dir, string &uid_dir) {

    string base_dir(BASE_DB_DIR);
    struct stat path_buf;
    base_dir = (base_dir + "/") + time_dir;
    if (stat(base_dir.c_str(), &path_buf) != 0) {
        mkdir (base_dir.c_str(), NEW_DIR_MODE);
        chmod (base_dir.c_str(), NEW_DIR_MODE);
    }   
    base_dir = (base_dir + "/") + uid_dir;
    if (stat(base_dir.c_str(), &path_buf) != 0) {
        mkdir (base_dir.c_str(), NEW_DIR_MODE);
        chmod (base_dir.c_str(), NEW_DIR_MODE);
    }

    string tune_opts = "#opts=cl#bnum=1000#psiz=32768";
    if (!db->open(base_dir + "/pg_agg_map_" + map_name + "." + DB_TYPE + tune_opts, g_sql_db_flags)) {
        cerr << "open error: " << db->error().name() << endl;
    }
    db->begin_transaction();
    // And make sure that everyone can write to this DB.
    chmod ((base_dir + "/pg_agg_map_" + map_name + "." + DB_TYPE).c_str(), FILE_MODE);
}

// For when we've tripped the size limit -- need to move all values from map_entry to the new db.
inline void dump_to_kc (cloudflare::ZoneTimeBucket &msg, PolyDB *current_map_kv, string &key, int64_t value) {
    for (int j = 0; j < msg.map_entry_size(); j++) {
        current_map_kv->increment(msg.map_entry(j).key(), msg.map_entry(j).value());
    }
    msg.clear_map_entry();
    current_map_kv->increment(key, value);
}

inline int handle_delete(PolyDB *deldb, string &line) {
    deldb->set(line, "1");
    return 0;
}

inline int handle_agg(PolyDB *aggdb, PolyDB *rid2tsdb, string &line, bool is_regular) {

    tokenizer tokens(line, CF_MAP_SEP);
    string rid;
    string uid;
    string time;
    int place = 0;
    bool found = true;

    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
        switch (place) {
        case 0: rid = (*tok_iter);
        case 2: uid = (*tok_iter);
        case 3: if (!is_regular) { CF_START_TIME_DUMP_RE.PartialMatch((*tok_iter), &time); } 
            else { CF_START_TIME_RE.PartialMatch((*tok_iter), &time); } 
        }
        place++;
    }
    
    if (found) {
        rid2tsdb->set(rid,  time + "\t" + uid);
    }
    aggdb->set(line, "1");
    return 0;
}

inline int handle_map(PolyDB *mapdb, string &line) {

    int                             place = MAP_RID;
    tokenizer                       tokens(line, CF_MAP_SEP);
    stringstream                    map_key;
    int64_t                         map_value = 0;

    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
        switch (place) {
        case MAP_RID: map_key << (*tok_iter) << "\t"; break; 
        case MAP_CLASSIFICATION: map_key << (*tok_iter) << "\t"; break;
        case MAP_DOCTYPE: map_key << (*tok_iter) << "\t"; break;
        case MAP_POP: map_key << (*tok_iter) << "\t"; break;
        case MAP_PSOURCE: map_key << (*tok_iter) << "\t"; break;
        case MAP_NAME: map_key << (*tok_iter) << "\t"; break;
        case MAP_KEY: map_key << (*tok_iter); break;
        case MAP_VALUE: map_value = kyotocabinet::atoi(tok_iter->c_str()); break;
        }
        place++;
    }
    
    if (map_value > 0) {
        mapdb->increment(map_key.str(), map_value);
    }
    return 0;
}

int load_file (PolyDB *deldb, PolyDB *aggdb, PolyDB *mapdb, PolyDB *ip_deldb, PolyDB *ipdb, PolyDB *rid2tsdb, string &file,
               int start_state = 0) {


    ifstream                            infile;
    string                              line;
    int                                 lines = 0;
    int                                 current_state = start_state;
    boost::iostreams::filtering_istream in;
    boost::iostreams::zlib_params       p;

    if (file.substr(file.length() - 3, 3) == ".gz") {
        infile.open(file.c_str(), ios_base::in | ios_base::binary);
        p.window_bits = 16 + MAX_WBITS;
        in.push(boost::iostreams::zlib_decompressor(p));
        in.push(infile);
    } else {
        infile.open(file.c_str(), ifstream::in);
        in.push(infile);
    }

    while (in.good()) {
        lines++;
        getline (in, line);
        if (CF_STATE_RE.PartialMatch(line)) {
            current_state += 10;
        } else {
            switch (current_state) {
            case DELETE: 
                handle_delete(deldb, line); break;
            case AGG: 
                handle_agg(aggdb, rid2tsdb, line, (start_state == 0)); break;
            case MAP: 
                handle_map(mapdb, line); break;
            }
        }
    }

    infile.close();    
    return lines;
}

int handle_map_merge(map<string, PolyDB *> &dbs, Json::Value &config, 
                     string &line, int64_t value, cloudflare::ZoneTimeBucket &msg,
                     PolyDB *current_map_kv, bool &db_open, PolyDB *rid2tsdb, PolyDB *dbs2queue) {

    stringstream                        map_db_key;
    stringstream                        kv_valwriter;
    char                                sizewriter[64];

    string                              classification;
    string                              rid;
    string                              map_name;
    string                              doctype;
    string                              pop;
    string                              psource;
    string                              key;

    tokenizer                           tokens(line, CF_MAP_SEP);
    int                                 place = MAP_RID;
    map<string, PolyDB *>::iterator     it;

    string                              map_db_key_str;
    string                              msg_value;
    string                              msg_ser;
    string                              time_dir;
    string                              uid_dir;
    string                              main_db_path;
    cloudflare::ZoneTimeBucket_Counter  *counter;
    string                              uid_time_dir;

    for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
        switch (place) {
        case MAP_RID: rid = (*tok_iter); break; 
        case MAP_CLASSIFICATION: classification = (*tok_iter); break;
        case MAP_DOCTYPE: doctype = (*tok_iter); map_db_key << (*tok_iter) << CF_LABEL_SEP; break;
        case MAP_POP: pop = (*tok_iter); map_db_key << (*tok_iter) << CF_LABEL_SEP; break;
        case MAP_PSOURCE: psource = (*tok_iter); map_db_key << (*tok_iter) << CF_LABEL_SEP; break;
        case MAP_NAME: map_name = (*tok_iter); map_db_key << (*tok_iter); break;
        case MAP_KEY: key = (*tok_iter); break;
        }
        place++;
    }

    // Do we need to make a new db?
    
    if (rid2tsdb->get(rid, &uid_time_dir)) {
        size_t tab_pos = uid_time_dir.find_first_of("\t");
        time_dir = uid_time_dir.substr(0, tab_pos);
        uid_dir = uid_time_dir.substr(tab_pos+1);
    } else {
        time_dir = DEFAULT_TIME_DIR;
        uid_dir = DEFAULT_UID_DIR; 
    }

    main_db_path = map_name + "/" + time_dir + "/" + uid_dir;
    it = dbs.find(main_db_path);
    if (it == dbs.end()) {
        if (dbs.size() > MAX_MAP_SIZE) {
            prune_map(dbs, dbs2queue, msg);
        }
        dbs[main_db_path] = new PolyDB(); // Need to clear out this listing periodically.
        open_main_db (dbs[main_db_path], map_name, time_dir, uid_dir);
        snprintf(sizewriter, 64, "%020ld", g_next_db);
        g_next_db++;
        dbs2queue->set(sizewriter, main_db_path);
    }

    map_db_key_str = rid + CF_LABEL_SEP + classification + CF_LABEL_SEP + map_db_key.str();
    //cout << "Going with " << map_db_key_str << endl;

    // If we are dealing with the same damn PB from before, use it.
    if (msg.has_db_key() && msg.db_key().compare(map_db_key_str) == 0) {

        //cout << "Reusing" << endl;

        // Clearing old values and resetting based on kc info from the merge.
        if (msg.has_kv_map_file()) {
            
            open_kv_db(current_map_kv, msg, db_open);            
            current_map_kv->increment(key, value);
            // cout << "Reusing current KC" << endl;

        } else if (msg.map_entry_size() > MAX_KEYS_BEFORE_KV_MAP) {
            // Set the map db path.
            set_kv_path(msg, map_db_key_str);            
            open_kv_db(current_map_kv, msg, db_open);
            dump_to_kc (msg, current_map_kv, key, value);

            // cout << "Dumping" << endl;

        } else {
            //            cout << "Setting " << key << endl;

            counter = msg.add_map_entry();
            counter->set_key(key);
            counter->set_value(value);
        }
    } else {
        // Save the old and get the new.
        // Save the old values (if they exist)
        if (msg.has_db_key()) {
            msg.SerializeToString(&msg_ser);
            dbs[msg.db_path()]->set(msg.db_key(), msg_ser);
            close_kv_db(current_map_kv, db_open);
        }

        // Load the old values if such a thing exists, updating the map keys accordingly.
        

        if (dbs[main_db_path]->get(map_db_key_str, &msg_value)) { 
            // Remove the old info from the map.
            msg.clear_map_entry();
            if (msg.ParseFromString(msg_value)) {   
                // Clearing old values and resetting based on kc info from the merge.
                if (msg.has_kv_map_file()) {
                    
                    //cout << "Opening" << msg.kv_map_file() << endl;

                    open_kv_db(current_map_kv, msg, db_open);
                    current_map_kv->increment(key, value);
                } else if (msg.map_entry_size() > MAX_KEYS_BEFORE_KV_MAP) {
                    set_kv_path(msg, map_db_key_str);
                    open_kv_db(current_map_kv, msg, db_open);

                    //cout << "Dumping from rebuild" << endl;

                    dump_to_kc (msg, current_map_kv, key, value);
                } else {
                    
                    //cout << "Adding counter " << key << endl;
                    counter = msg.add_map_entry();
                    counter->set_key(key);
                    counter->set_value(value);
                }
            }
        } else { 

            //cout << "Adding counter fresh msg " << key << endl;

            msg.set_result_id(rid);
            msg.set_map_name(map_name);
            msg.set_classification(classification);
            msg.set_doctype(doctype);
            msg.set_pop(pop);
            msg.set_psource(psource);
            msg.set_db_key(map_db_key_str);
            msg.set_db_path(main_db_path);

            // Assumed to be a new entry, so free to just go crazy.
            msg.clear_map_entry();
            msg.clear_kv_map_file();
            counter = msg.add_map_entry();
            counter->set_key(key);
            counter->set_value(value);
        }
    }

    return 1; // Why one? why not?
}

int output_merged (PolyDB *deldb, PolyDB *aggdb, PolyDB *mapdb, PolyDB *ip_deldb, PolyDB *ipdb, PolyDB *rid2tsdb, PolyDB *dbs2queue, 
                   Json::Value &config, string &tmp_file, string &final_file) {

    ofstream                          outfile (tmp_file.c_str(), ios_base::out | ios_base::trunc);
    DB::Cursor                        *cur = deldb->cursor();
    string                            rec_key;
    string                            map_key;
    map<string, PolyDB *>             dbs;
    map<string, PolyDB *>::iterator   it;
    cloudflare::ZoneTimeBucket        current_msg;
    string                            current_msg_ser;

    outfile << "BEGIN;\n";

    cur->jump();
    while (cur->get_key(&rec_key, true)) {
        outfile << rec_key << endl;
    }
    delete cur;

    outfile << "COPY aggregate_meta_data (result_id, zid, oid, start_time, period, zone_name) FROM stdin WITH DELIMITER E'\\t';" 
            << endl;

    cur = aggdb->cursor();
    cur->jump();
    while (cur->get_key(&rec_key, true)) {
        outfile << rec_key << endl;
    }
    delete cur;

    outfile << "\\.\n";

    // Move the map values into a kcdb here.
    cur = mapdb->cursor();
    cur->jump();

    PolyDB kv_map_db;
    bool db_open = false;

    while (cur->get_key(&map_key, true)) {
        handle_map_merge(dbs, config, map_key, mapdb->increment(map_key, 0), current_msg, &kv_map_db, db_open, 
                         rid2tsdb, dbs2queue);
    }
    delete cur;

    close_kv_db(&kv_map_db, db_open);

    // Save the last value to the db.
    if (current_msg.has_map_name()) {
        current_msg.SerializeToString(&current_msg_ser);
        dbs[current_msg.db_path()]->set(current_msg.db_key(), current_msg_ser);
    }

    outfile << "COMMIT;\n";
  
    // Close all of the agg map dbs.
    for ( it = dbs.begin() ; it != dbs.end(); it++ ) {
        it->second->end_transaction(1);
        it->second->close();
        delete it->second;
    }

    outfile.close();

    // Now, rename to final.
    rename (tmp_file.c_str(), final_file.c_str());

    return 1;
}

inline void open_dbs(PolyDB &deldb, PolyDB &aggdb, PolyDB &mapdb, PolyDB &ip_deldb, PolyDB &ipdb, PolyDB &rid2tsdb, PolyDB &dbs2queue, 
                     Json::Value &config, const uint32_t db_flags) {

    if (!deldb.open(config.get("LOG_MERGE_DELETE", "orig_del.kct").asString(), db_flags)) {
        cerr << "open error (deldb): " << deldb.error().name() << endl;
    }
    if (!aggdb.open(config.get("LOG_MERGE_AGG", "orig_agg.kct").asString(), db_flags)) {
        cerr << "open error (aggdb): " << aggdb.error().name() << endl;
    }
    if (!mapdb.open(config.get("LOG_MERGE_MAP", "orig_map.kct").asString(), db_flags)) {
        cerr << "open error (mapdb): " << mapdb.error().name() << endl;
    }
    if (!ip_deldb.open(config.get("LOG_MERGE_DELETE_IPS", "orig_del_ips.kct").asString(), db_flags)) {
        cerr << "open error (ip_deldb): " << ip_deldb.error().name() << endl;
    }
    if (!ipdb.open(config.get("LOG_MERGE_IPS", "orig_ipdb.kct").asString(), db_flags)) {
        cerr << "open error (ipdb): " << ipdb.error().name() << endl;
    }
    if (!rid2tsdb.open("%", db_flags)) {
        cerr << "open error: " << rid2tsdb.error().name() << endl;
    }
    if (!dbs2queue.open("%", db_flags)) {
        cerr << "open error: " << dbs2queue.error().name() << endl;
    }
}

inline void close_dbs(PolyDB &deldb, PolyDB &aggdb, PolyDB &mapdb, PolyDB &ip_deldb, PolyDB &ipdb, PolyDB &rid2tsdb, PolyDB &dbs2queue) {
    deldb.close();
    aggdb.close();
    mapdb.close();
    ip_deldb.close();
    ipdb.close();
    rid2tsdb.close();
    dbs2queue.close();
}

int main(int argc, char** argv) {

    const char      *config_file = "importer.json";
    const char      *lock_file = "/tmp/import.LOCK"; // This is important.
    int             c;
    struct stat     path_buf;
    string          current_file;
    const uint32_t  db_flags = PolyDB::OWRITER | PolyDB::OCREATE | PolyDB::OTRUNCATE;
    string          tmp_file;
    string          final_file;
    stringstream    file_namer;
    int             start_state = 0;

    while (-1 != (c = getopt(argc, argv, "c:d"))) {
        switch (c) {
        case 'd':
            start_state = DELETE;
            break;
        case 'c':
            config_file = optarg;
            break;
        default:
            exit(-22);
        }
    }

    // DBs here for the merging part.
    PolyDB deldb;
    PolyDB aggdb;
    PolyDB mapdb;
    PolyDB ip_deldb;
    PolyDB ipdb;
    PolyDB rid2tsdb;
    PolyDB dbs2queue;

    // Grab the config file.
    Json::Value              config;
    Json::Reader             reader;
    ifstream                 inconf(config_file, ifstream::in);

    // Check the lock file.
    if (stat(lock_file, &path_buf) == 0) {
        cerr << "ERROR: Lockfile " << lock_file << " present." << endl;
        exit(-22);
    } else {
        ofstream lockstr;
        lockstr.open (lock_file);
        lockstr.close();
    }
    
    if (!reader.parse(inconf, config)) {
        return -1;
    }
    inconf.close();

    // Make sure that the proto bufs are happy.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    open_dbs(deldb, aggdb, mapdb, ip_deldb, ipdb, rid2tsdb, dbs2queue, config, db_flags);

    // Parse the file(s)
    int nextarg = optind;
    if (argc > optind) {
        while (nextarg < argc) {
            if (stat(argv[nextarg], &path_buf) == 0) {
                if (S_ISREG(path_buf.st_mode)) {
                    current_file = string(argv[nextarg]);
                    load_file(&deldb, &aggdb, &mapdb, &ip_deldb, &ipdb, &rid2tsdb, current_file, start_state);
                } else if (S_ISDIR(path_buf.st_mode)) {
                    DIR                *dir;
                    struct dirent      *ent;
                    string             tmp_file;
                    
                    current_file = string(argv[nextarg]);
                    dir = opendir (argv[nextarg]);
                    if (dir != NULL) {
                        while ((ent = readdir (dir)) != NULL) {
                            tmp_file = current_file + "/" + ent->d_name;
                            load_file(&deldb, &aggdb, &mapdb, &ip_deldb, &ipdb, &rid2tsdb, tmp_file, start_state);
                        }
                        closedir (dir);
                    }
                }
            }
            nextarg++;
        }
    }

    // Write out the new file
    file_namer << config.get("SQL_FILE_EXTRA_NEW_DIR", "/tmp").asString() << "/tmp_" << time(NULL) << ".sql";
    tmp_file = file_namer.str();
    file_namer.str("");

    file_namer << config.get("SQL_NEW_FILE_DIR", "/tmp").asString() << "/final_" << time(NULL) << ".sql";
    final_file = file_namer.str();

    output_merged(&deldb, &aggdb, &mapdb, &ip_deldb, &ipdb, &rid2tsdb, &dbs2queue, config, tmp_file, final_file);
    cout << final_file << endl;
    
    // And the PB.
    google::protobuf::ShutdownProtobufLibrary();

    close_dbs(deldb, aggdb, mapdb, ip_deldb, ipdb, rid2tsdb, dbs2queue);

    // And the lock file.
    unlink (lock_file);
}
