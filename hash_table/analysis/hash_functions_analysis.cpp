#include "hash_functions_analysis.h"

int AnalyzeMultipleHashFuncsFile (const char * const * const res_paths,
                                  const char * const         preprocessed_target_data_path,
                                  const hash_func_ptr_t      *hash_funcs,
                                  const size_t               hash_table_size,
                                  const size_t               n_hash_functions)
{
    FILE *preprocessed_target_data_file = fopen (preprocessed_target_data_path, "rb");
    int fsize = GetFileSize (preprocessed_target_data_file);

    char *preprocessed_target_data_buf = (char *) calloc (fsize + 1, sizeof (char));

    fread (preprocessed_target_data_buf, sizeof (char), fsize, preprocessed_target_data_file);

    AnalyzeMultipleHashFunctions (res_paths, preprocessed_target_data_buf, fsize + 1, hash_funcs, 1001, n_hash_functions);

    fclose (preprocessed_target_data_file);
    free (preprocessed_target_data_buf);

    return 0;
}

int AnalyzeMultipleHashFunctions (const char * const    *analysis_results_paths,
                                  const char            *preprocessed_target_data,
                                  size_t                target_data_size,
                                  const hash_func_ptr_t *hash_functions,
                                  size_t                hash_table_size,
                                  size_t                n_hash_functions)
{
    assert (analysis_results_paths);
    assert (preprocessed_target_data);
    assert (hash_functions);

    for (int n_hf = 0; n_hf < n_hash_functions; n_hf++)
    {
        PrintProgressBar (n_hf, n_hash_functions);

        AnalyzeHashFunction (analysis_results_paths[n_hf],
                             preprocessed_target_data,
                             target_data_size,
                             hash_functions[n_hf],
                             hash_table_size);

        PrintProgressBar (n_hf + 1, n_hash_functions);
    }

    fprintf (stderr, "\n");

    return 0;
}

int AnalyzeHashFunction (const char * const analysis_results_path,
                         const char         *preprocessed_target_data,
                         size_t             target_data_size,
                         hash_func_ptr_t    hash_function,
                         size_t             hash_table_size)
{
    assert (analysis_results_path);
    assert (preprocessed_target_data);
    assert (hash_function);

    int ret_val = 0;

    HashTable *hash_table = HashTableCtor (hash_table_size, hash_function);

    char *target_data_copy = (char *) calloc (target_data_size, sizeof (char));

    FILE *results_file = fopen (analysis_results_path, "wb");

    if (!results_file)
    {
        ERROR ("Couldnt open file \"%s\"", analysis_results_path);
        ret_val = 1;
        goto cleanup_and_return;
    };

    strcpy (target_data_copy, preprocessed_target_data);

    HashTableLoadTargetData (hash_table, target_data_copy, '\n');

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