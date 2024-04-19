#include "hash_functions_analysis.h"

int AnalyzeMultipleHashFunctions (const char * const * const analysis_results_paths,
                                  FILE                       *preprocessed_target_data_file,
                                  const hash_func_ptr_t      *hash_functions,
                                  const size_t               n_hash_functions,
                                  const size_t               hash_table_size)
{
    assert (analysis_results_paths);
    assert (preprocessed_target_data_file);
    assert (hash_functions);

    int ret_val = 0;

    size_t fsize = GetFileSize (preprocessed_target_data_file);

    char *preprocessed_target_data_buf = (char *) calloc (fsize + 1, sizeof (char));

    if (!preprocessed_target_data_buf)
    {
        ret_val = -1;
        goto cleanup_and_return;
    }
    // ignore ret value
    (void)!fread (preprocessed_target_data_buf, sizeof (char), fsize, preprocessed_target_data_file);

    for (size_t n_hf = 0; n_hf < n_hash_functions; n_hf++)
    {
        PrintProgressBar (n_hf, n_hash_functions);

        ret_val += AnalyzeHashFunction (analysis_results_paths[n_hf],
                                        preprocessed_target_data_buf,
                                        fsize + 1,
                                        hash_functions[n_hf],
                                        hash_table_size);

        PrintProgressBar (n_hf + 1, n_hash_functions);
    }

    fprintf (stderr, "\n"); // new line for after progress bar printing

cleanup_and_return:
    fclose (preprocessed_target_data_file);
    free (preprocessed_target_data_buf);

    return ret_val;
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
        ERROR ("Couldnt open file \"%s\". Skipping current test...", analysis_results_path);
        ret_val = 1;
        goto cleanup_and_return;
    };

    strcpy (target_data_copy, preprocessed_target_data);

    HashTableLoadTargetData (hash_table, target_data_copy, '\n');

    fprintf (results_file, "%s, %s\n", "id", "n");
    for (size_t id = 0; id < hash_table_size; id++)
        fprintf (results_file, "%lu, %d\n", id,
                            (hash_table->table[id]) ? hash_table->table[id]->cells_used : 0);

cleanup_and_return:
    fclose (results_file);
    HashTableDtor (hash_table);

    return ret_val;
}

int TestPerformanceMultipleHashFunctions (FILE                          *test_results_file,
                                          FILE                          *test_cases_file,
                                          const char * const            *tests_names,
                                          FILE                          *preprocessed_target_data_file,
                                          const hash_func_ptr_t * const hash_functions,
                                          size_t                        n_hash_functions,
                                          size_t                        hash_table_size)
{
    assert (test_results_file);
    assert (test_cases_file);
    assert (tests_names);
    assert (preprocessed_target_data_file);
    assert (hash_functions);

    int ret_val = 0;

    size_t fsize = GetFileSize (preprocessed_target_data_file);

    char *preprocessed_target_data_buf = (char *) calloc (fsize + 1, sizeof (char));

    if (!preprocessed_target_data_buf)
    {
        ERROR ("Target data buffer allocation error");
        ret_val = -1;
        goto cleanup_and_return;
    }
    // ignore ret value
    (void)!fread (preprocessed_target_data_buf, sizeof (char), fsize, preprocessed_target_data_file);

    fprintf (test_results_file, "name, mean, var, min_v, max_v\n");
    LOG ("Running tests...");
    for (size_t n_hf = 0; n_hf < n_hash_functions; n_hf++)
    {
        TestPerformanceHashFunction (test_results_file,
                                     test_cases_file,
                                     tests_names[n_hf],
                                     preprocessed_target_data_buf,
                                     hash_functions[n_hf],
                                     hash_table_size);

        rewind (test_cases_file);
    }

    fprintf (stderr, "\n"); // new line for after progress bar printing

    LOG ("Performance testing finished");

cleanup_and_return:

    free (preprocessed_target_data_buf);

    return ret_val;
}

int TestPerformanceHashFunction (FILE               *test_results_file,
                                 FILE               *test_cases_file,
                                 const char * const test_name,
                                 const char         *preprocessed_target_data,
                                 hash_func_ptr_t    hash_function,
                                 size_t             hash_table_size)
{
    assert (test_results_file);
    assert (test_cases_file);
    assert (test_name);
    assert (preprocessed_target_data);
    assert (hash_function);

    int ret_val = 0;

    HashTable *hash_table = HashTableCtor (hash_table_size, hash_function);

    LOG ("hash table for \"%s\" test created", test_name);

    char *target_data_copy = strdup (preprocessed_target_data);

    LOG ("Loading target data...");

    HashTableLoadTargetData (hash_table, target_data_copy, '\n');

    LOG ("target data loaded");

    RunPerformanceTestHashTable (
        test_results_file,
        test_cases_file,
        hash_table,
        test_name
    );

    free (target_data_copy);
    HashTableDtor (hash_table);

    LOG ("hash table for \"%s\" test destroyed", test_name);

    return ret_val;
}


int RunPerformanceTestHashTable (FILE               *test_results_file,
                                 FILE               *test_cases_file,
                                 HashTable          *hash_table,
                                 const char * const test_name)
{
    assert (test_results_file);
    assert (test_cases_file);
    assert (hash_table);
    assert (test_name);

    size_t n_test      = 0;
    size_t max_n_tests = 0;
    char key[MAX_WORD_LEN] = {};

    size_t mean_val     = 0;
    double variance     = 0;
    size_t sum          = 0;
    size_t sqr_delta    = 0;
    size_t min_test_res = -1;
    size_t max_test_res = 0;

    if (fscanf (test_cases_file, "%lu", &max_n_tests) != 1)
        ERROR ("Could not read number of tests. Wrong file format!");

    u_int64_t *test_results = (u_int64_t *) calloc (max_n_tests, sizeof (u_int64_t));

    fprintf (test_results_file, "%s, ", test_name);

    LOG ("Start testing \"%s\" (%lu testcases, %lu ht n elems)", test_name, max_n_tests, hash_table->n_elems);

    while (fscanf (test_cases_file, "%s\n", key) == 1)
    {
        u_int64_t test_res = -GetCPUTicks ();

        HashTableGetVal (hash_table, key);

        test_res += GetCPUTicks ();

        test_results[n_test] = test_res;

        if (test_res > max_test_res) max_test_res = test_res;
        if (test_res < min_test_res) min_test_res = test_res;

        sum += test_res; // contains sum of elements

        n_test++;
    }

    mean_val = sum / (n_test + 1);

    LOG ("Test results data filled, calculating variance...");

    for (size_t i = 0; i <= n_test; i++)
    {
        size_t delta = test_results[i] - mean_val;
        sqr_delta += delta * delta;
    }

    variance = sqrt( sqr_delta / (n_test + 1));

    fprintf (test_results_file, "%lu, %lg, %lu, %lu\n", mean_val, variance, min_test_res, max_test_res);

    free (test_results);

    LOG ("Testing \"%s\" finished", test_name);

    return n_test;
}

size_t GetFileSize (FILE *file)
{
    if (!file) return 0;

    size_t fsize = 0;

    fseek (file, 0, SEEK_END);
    fsize = ftell (file);
    rewind (file);

    return fsize;
}
