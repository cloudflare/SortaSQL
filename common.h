#ifndef CF_COMMON_KCPG
#define CF_COMMON_KCPG

#include <sys/types.h>

static const char *BASE_DB_DIR = "/mnt/ssd/pg/kyoto";
static const char *DB_TYPE = "kct";
static const mode_t NEW_DIR_MODE = 0777;
static const mode_t NEW_DIR_MASK = 0777;
static const mode_t FILE_MODE = 0666;
static const char *CF_LABEL_SEP = ":";
static const int MAX_KEYS_BEFORE_KV_MAP = 512; // How many keys to keep as a counter, before placing them in KC.
//static const int MAX_KEYS_BEFORE_KV_MAP = 1; // How many keys to keep as a counter, before placing them in KC.
static const int KC_MAX_ENTRIES_PER_RID = 64;

// Where to stick kv dbs.
static const char *CF_KV_MAP_BASE = "/mnt/ssd_new/pg/kyoto_kvs";

#endif
