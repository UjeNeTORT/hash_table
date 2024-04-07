#ifndef HASH_FUNCTIONS_ANALYSIS
#define HASH_FUNCTIONS_ANALYSIS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_table.h"
#include "../hash_functions/hash_functions.h"

int AnalyzeMultipleHashFunctions (const char * const * const analysis_results_paths,
                                  const char * const         preprocessed_target_data_path,
                                  const hash_func_ptr_t      *hash_functions,
                                  const size_t               hash_table_size,
                                  const size_t               n_hash_functions);

int AnalyzeHashFunction (const char * const analysis_results_path,
                         const char         *preprocessed_target_data,
                         size_t             target_data_size,
                         hash_func_ptr_t    hash_function,
                         size_t             hash_table_size);

size_t GetFileSize (FILE *file);

#endif // HASH_FUNCTIONS_ANALYSIS