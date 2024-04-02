#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/common.h"
#include "../linked_list/super_list.h"

const size_t DEFAULT_LIST_SIZE = 1;

struct HashTable
{
    List    **table;
    size_t  size;
    volatile u_int64_t (*hash_func_ptr)
            (const char * const word, size_t word_length);
};

HashTable *HashTableCtor   (size_t hash_table_size,
                            volatile u_int64_t (* hash_func_ptr)
                                    (const char * const word, size_t word_length));

void       HashTableDtor   (HashTable *hash_table);

int        HashTableVerifier ();
int        HashTableDump     ();

#endif // HASH_TABLE_H