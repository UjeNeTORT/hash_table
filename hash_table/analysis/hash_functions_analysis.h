#ifndef HASH_FUNCTIONS_ANALYSIS
#define HASH_FUNCTIONS_ANALYSIS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_table.h"
#include "../hash_functions/hash_functions.h"

int AnalyzeHashFunction (const char * const analysis_results_path,
                         char               *preprocessed_target_data,
                         hash_func_ptr_t    hash_function,
                         size_t             hash_table_size);

int GetFileSize (FILE *file);


#endif // HASH_FUNCTIONS_ANALYSIS