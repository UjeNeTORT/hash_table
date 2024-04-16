#ifndef HASH_FUNCTIONS_ANALYSIS
#define HASH_FUNCTIONS_ANALYSIS

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_table.h"
#include "../hash_functions/hash_functions.h"

int AnalyzeMultipleHashFunctions
(
    const char * const * const analysis_results_paths,
    FILE                       *preprocessed_target_data_file,
    const hash_func_ptr_t      *hash_functions,
    const size_t               n_hash_functions,
    const size_t               hash_table_size
);

int AnalyzeHashFunction
(
    const char * const analysis_results_path,
    const char         *preprocessed_target_data,
    size_t             target_data_size,
    hash_func_ptr_t    hash_function,
    size_t             hash_table_size
);

int TestPerformanceMultipleHashFunctions
(
    FILE                          *test_results_file,
    FILE                          *test_cases_file,
    const char * const            *tests_names,
    FILE                          *preprocessed_target_data_file,
    const hash_func_ptr_t * const hash_functions,
    size_t                        n_hash_functions,
    size_t                        hash_table_size
);

int TestPerformanceHashFunction
(
    FILE               *test_results_file,
    FILE               *test_cases_file,
    const char * const test_name,
    const char         *preprocessed_target_data,
    hash_func_ptr_t    hash_function,
    size_t             hash_table_size
);

int RunPerformanceTestHashTable
(
    FILE               *test_results_file,
    FILE               *test_cases_file,
    HashTable          *hash_table,
    const char * const tests_name
);

size_t GetFileSize (FILE *file);

#endif // HASH_FUNCTIONS_ANALYSIS