#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../config.h"
#include "../common/common.h"
#include "../linked_list/super_list.h"

const size_t MAX_N_LINES          = 5 * 1e5;
const size_t EXPECTED_WORD_LENGTH = 6;

struct HashTable
{
    List            **table;
    size_t          size;
    size_t          n_elems;
    hash_func_ptr_t hash_func;
};

HashTable  *HashTableCtor       (size_t          hash_table_size,
                                 hash_func_ptr_t hash_func);

void        HashTableDtor       (HashTable *hash_table);

int         HashTableInsert     (HashTable *hash_table, ht_key_t key);
int         HashTableRemove     (HashTable *hash_table, ht_key_t key);

int         HashTableGetVal     (HashTable *hash_table, ht_key_t key);

int HashTableLoadTargetData     (HashTable *hash_table,
                                 char      *delimited_text_buff,
                                 int        delimiter_char);

int HashTableLoadTargetDataAligned (HashTable  *hash_table,
                                    const char *aligned_text_buff,
                                    size_t     buf_size);

int FillAlignedBuf (char *ptd_aligned,
                    char *preprocessed_target_data_buf,
                    char delimiter_char,
                    size_t n_words);

#endif // HASH_TABLE_H
