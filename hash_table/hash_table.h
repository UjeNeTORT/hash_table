#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../config.h"
#include "../common/common.h"
#include "../linked_list/super_list.h"

const size_t DEFAULT_LIST_SIZE = 1;

struct HashTable
{
    List            **table;
    size_t          size;
    hash_func_ptr_t hash_func;
};

HashTable  *HashTableCtor       (size_t          hash_table_size,
                                 hash_func_ptr_t hash_func);

void        HashTableDtor       (HashTable *hash_table);

int         HashTableInsert     (HashTable *hash_table, ht_key_t key);
int         HashTableRemove     (HashTable *hash_table, ht_key_t key);

int         HashTableGetVal     (HashTable *hash_table, ht_key_t key);

int         HashTableVerifier   ();
int         HashTableDump       ();

list_elem_t GetTempListEl       (ht_key_t key);

#endif // HASH_TABLE_H