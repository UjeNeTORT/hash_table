#include <stdio.h>

#include "hash_table/hash_table.h"
#include "hash_table/hash_functions/hash_functions.h"
#include "hash_table/analysis/hash_functions_analysis.h"

int main ()
{
    const char * const res_paths[]
                               = {"hash_table/analysis/results/csv/retzero_hf.csv",
                                  "hash_table/analysis/results/csv/ascii_first_hf.csv",
                                  "hash_table/analysis/results/csv/wd_len_hf.csv",
                                  "hash_table/analysis/results/csv/ctrl_sum_hf.csv",
                                  "hash_table/analysis/results/csv/sum_by_len_hf.csv",
                                  "hash_table/analysis/results/csv/crc32_hf.csv"};

    hash_func_ptr_t hash_funcs[] = {CalcStrHashReturnZero,
                                    CalcStrHashFirstLetter,
                                    CalcStrHashWordLength,
                                    CalcStrHashControlSum,
                                    CalcStrHashSumOverLength,
                                    CalcStrHashCRC32};

    AnalyzeMultipleHashFunctions (res_paths, "texts/preprocessed/GeorgeRRMartin.txt", hash_funcs, 1001, sizearr (hash_funcs));

    return 0;
}