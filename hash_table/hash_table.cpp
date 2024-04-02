#include "hash_table.h"

HashTable *HashTableCtor (
    size_t hash_table_size,
    volatile u_int64_t (* hash_func_ptr)
            (const char * const word, size_t word_length))
{
    HashTable *hash_table = (HashTable *) calloc (1, sizeof (HashTable));

    hash_table->table = (List **) calloc (hash_table_size, sizeof (List *));

    for (int i = 0; i < hash_table_size; i++)
        hash_table->table[i] = ListCtor (DEFAULT_LIST_SIZE);

    hash_table->size = hash_table_size;

    hash_table->hash_func_ptr = hash_func_ptr;

    return hash_table;
}

void HashTableDtor (HashTable *hash_table)
{
    assert (hash_table);

    for (int i = 0; i < hash_table->size; i++)
        ListDtor (hash_table->table[i]);

    hash_table->size = 0;
    hash_table->hash_func_ptr = NULL;

    free (hash_table->table);
    free (hash_table);

    return; // void
}

