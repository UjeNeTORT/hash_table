#include "hash_functions_analysis.h"

int AnalyzeHashFunction (const char * const analysis_results_path,
                         char               *preprocessed_target_data,
                         hash_func_ptr_t    hash_function,
                         size_t             hash_table_size)
{
    assert (analysis_results_path);
    assert (preprocessed_target_data);
    assert (hash_function);

    int ret_val = 0;

    HashTable *hash_table = HashTableCtor (hash_table_size, hash_function);

    FILE *results_file = fopen (analysis_results_path, "wb");
    if (!results_file)
    {
        ERROR ("Couldnt open file \"%s\"", analysis_results_path);
        ret_val = 1;
        goto cleanup_and_return;
    };

    HashTableLoadTargetData (hash_table, preprocessed_target_data, '\n');

    fprintf (results_file, "%s, %s\n", "id", "n");
    for (int id = 0; id < hash_table_size; id++)
        fprintf (results_file, "%d, %d\n", id,
                            (hash_table->table[id]) ? hash_table->table[id]->cells_used : 0);

cleanup_and_return:
    fclose (results_file);
    HashTableDtor (hash_table);

    return ret_val;
}

int GetFileSize (FILE *file)
{
    if (!file) return 0;

    int fsize = 0;

    fseek (file, 0, SEEK_END);
    fsize = ftell(file);
    rewind (file);

    return fsize;
}