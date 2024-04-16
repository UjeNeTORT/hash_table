#include "hash_table.h"

HashTable *HashTableCtor (
    size_t          hash_table_size,
    hash_func_ptr_t hash_func_ptr)
{
    HashTable *hash_table = (HashTable *) calloc (1, sizeof (HashTable));

    hash_table->table = (List **) calloc (hash_table_size + 1, sizeof (List *));

    // TODO check if size is prime

    hash_table->size = hash_table_size;

    hash_table->hash_func = hash_func_ptr;

    return hash_table;
}

void HashTableDtor (HashTable *hash_table)
{
    assert (hash_table);

    for (int i = 0; i < hash_table->size; i++)
        if (hash_table->table[i]) ListDtor (hash_table->table[i]);

    hash_table->size = 0;
    hash_table->n_elems = 0;
    hash_table->hash_func = NULL;

    free (hash_table->table);
    free (hash_table);

    return; // void
}

int HashTableGetVal (HashTable *hash_table, ht_key_t key)
{
    assert (hash_table);
    assert (key);

    hash_t hash = hash_table->hash_func (key) % hash_table->size;

    int elem_id = -1;

    if (hash_table->table[hash])
        elem_id = GetIdListKey (hash_table->table[hash], key);

    if (elem_id == -1) return LIST_POISON.value;

    list_elem_t l_elem = GetElemListId (hash_table->table[hash], elem_id);

    return l_elem.value;
}

int HashTableInsert (HashTable *hash_table, ht_key_t key)
{
    assert (hash_table);
    assert (key);

    hash_t hash = hash_table->hash_func (key) % hash_table->size;

    if (!hash_table->table[hash])
    {
        hash_table->table[hash] = ListCtor (DEFAULT_LIST_SIZE);

        int ret_val = InsertEndList (hash_table->table[hash], key);
        hash_table->n_elems++;

        return ret_val;
    }

    int elem_id = GetIdListKey (hash_table->table[hash], key);

    if (elem_id == -1)
    {
        InsertEndList (hash_table->table[hash], key);
        hash_table->n_elems++;
    }

    int ret_val = IncreaseValListId (hash_table->table[hash], elem_id);

    return ret_val;
}

int HashTableLoadTargetData (HashTable *hash_table,
                             char      *delimited_text_buff,
                             int        delimiter_char)
{
    assert (hash_table);
    assert (delimited_text_buff);

    int n_line = 0;
    char *buf_pos = delimited_text_buff;

    for (n_line = 0; n_line < MAX_N_LINES; n_line++)
    {
        char *curr_word = buf_pos;

        buf_pos = strchr (buf_pos, delimiter_char);
        if (!buf_pos) break; // no delimiters left => break
        *buf_pos = 0; // delim char -> \0
        buf_pos++;    // next letter

        HashTableInsert (hash_table, curr_word);
    }
    // if (n_line == MAX_N_LINES - 1)
        // WARN ("only MAX_N_LINES = %llu can be readen, all the rest will be ignored", MAX_N_LINES);

    return n_line + 1; // number of readen lines
}
