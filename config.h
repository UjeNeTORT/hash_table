#ifndef CONFIG_H
#define CONFIG_H

// #define LIST_DEBUG

typedef const char * ht_key_t;
typedef u_int64_t (* hash_func_ptr_t)(ht_key_t key);
typedef u_int64_t          hash_t;

typedef struct
{
    ht_key_t key;
    int      value;
} list_elem_t;

const int         PTD_ALIGNMENT           = 32;

const list_elem_t LIST_POISON             = {"pzn_data", -1};
const size_t      DEFAULT_LIST_SIZE       = 1;
const int         DEFAULT_LIST_ELEM_VALUE = 1;

const int         MAX_WORD_LEN            = 50;

const size_t      PROGRESS_BAR_SYNC_PERIOD = 1e5;

#endif // CONFIG_H
