#ifndef CONFIG_H
#define CONFIG_H

typedef const char * const ht_key_t;
typedef volatile u_int64_t (* hash_func_ptr_t)(ht_key_t key);
typedef u_int64_t          hash_t;

typedef struct
{
    ht_key_t word;
    int      value;
} list_elem_t;

const list_elem_t LIST_POISON = {"pzn_data", -1};

const int DEFAULT_LIST_ELEM_VALUE = 1;

#endif // CONFIG_H