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

    for (size_t i = 0; i < hash_table->size; i++)
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
    else
        return LIST_POISON.value;

    return GetValListId (hash_table->table[hash], elem_id);
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

    size_t n_line = 0;
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

    return n_line + 1; // number of readen lines
}

int HashTableLoadTargetDataAligned (HashTable *hash_table,
                                    char      *aligned_text_buff,
                                    size_t    buf_size)
{
    assert (hash_table);
    assert (aligned_text_buff);

    size_t n_line = 0;

    char *buf_pos = aligned_text_buff;

    for (n_line = 0; n_line < MAX_N_LINES; n_line++)
    {
        char *curr_word = buf_pos;

        HashTableInsert (hash_table, curr_word);

        int curr_wd_len = strlen (curr_word);
        buf_pos += curr_wd_len + 1;
        while (!*buf_pos)
        {
            if (buf_pos >= aligned_text_buff + buf_size) goto func_end;
            buf_pos++;
        }

        if (buf_pos >= aligned_text_buff + buf_size) break;
    }

func_end:

    return n_line + 1; // number of readen lines
}


int FillAlignedBuf (char *ptd_aligned,
                    char *preprocessed_target_data_buf,
                    char delimiter_char,
                    size_t n_words)
{
    assert (ptd_aligned);
    assert (preprocessed_target_data_buf);

    char *ptd_aligned_tmp = ptd_aligned;
    char *ptd_tmp         = strchr (preprocessed_target_data_buf, delimiter_char) + 1; // skip first line - number of words

    for (int i = 0; i < n_words; i++)
    {
        char *delim_pos = strchr (ptd_tmp, delimiter_char);
        if (!delim_pos) break;
        *delim_pos = 0;

        strcpy (ptd_aligned_tmp, ptd_tmp);
        int curr_wd_len = strlen (ptd_tmp);
        ptd_tmp += curr_wd_len + 1;
        ptd_aligned_tmp += curr_wd_len + 1;

        while ((int)(ptd_aligned_tmp - ptd_aligned) % PTD_ALIGNMENT) *(ptd_aligned_tmp++) = 0;
    }

    return 0;
}