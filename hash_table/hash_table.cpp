#include "hash_table.h"

HashTable *HashTableCtor (
    size_t hash_table_size,
    hash_func_ptr_t hash_func_ptr)
{
    HashTable *hash_table = (HashTable *) calloc (1, sizeof (HashTable));

    hash_table->table = (List **) calloc (hash_table_size, sizeof (List *));

    // for (int i = 0; i < hash_table_size; i++)
    //     hash_table->table[i] = ListCtor (DEFAULT_LIST_SIZE);

    hash_table->size = hash_table_size;

    hash_table->hash_func = hash_func_ptr;

    return hash_table;
}

void HashTableDtor (HashTable *hash_table)
{
    assert (hash_table);

    for (int i = 0; i < hash_table->size; i++)
        ListDtor (hash_table->table[i]);

    hash_table->size = 0;
    hash_table->hash_func = NULL;

    free (hash_table->table);
    free (hash_table);

    return; // void
}

int HashTableGetVal (HashTable *hash_table, ht_key_t key)
{
    assert (hash_table);
    assert (key);

    hash_t hash = hash_table->hash_func (key);

    int elem_id = GetIdListKey (hash_table->table[hash], key);

    if (elem_id == -1) return LIST_POISON.value;

    list_elem_t l_elem = GetElemListId (hash_table->table[hash], elem_id);

    return l_elem.value;
}

int HashTableInsert (HashTable *hash_table, ht_key_t key)
{
    assert (hash_table);
    assert (key);

    hash_t hash = hash_table->hash_func (key);

    if (!hash_table->table[hash])
    {
        hash_table->table[hash] = ListCtor (DEFAULT_LIST_SIZE);

        int ret_val = InsertBeginList (hash_table->table[hash], key);

        ListDump (DOT_DUMP_FILENAME, hash_table->table[hash], 0, {NULL, NULL, NULL, 0}); // todelete

        return ret_val;
    }

    int elem_id = GetIdListKey (hash_table->table[hash], key);

    int ret_val =  IncreaseValListId (hash_table->table[hash], elem_id);

    ListDump (DOT_DUMP_FILENAME, hash_table->table[hash], 0, {NULL, NULL, NULL, 0}); // todelete

    return ret_val;
}

// int HashTableRemove (HashTable *hash_table, ht_key_t key);

list_elem_t GetTempListEl (ht_key_t key)
{
    assert (key);

    list_elem_t temp_list_el = {key, LIST_POISON.value};

    return temp_list_el;
}