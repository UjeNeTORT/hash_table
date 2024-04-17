#include <stdio.h>

// #define NDEBUG

#include "hash_table/hash_table.h"
#include "hash_table/hash_functions/hash_functions.h"
#include "hash_table/analysis/hash_functions_analysis.h"

const hash_func_ptr_t HASH_FUNCS[] =
{
    CalcStrHashReturnZero,
    CalcStrHashFirstLetter,
    CalcStrHashWordLength,
    CalcStrHashControlSum,
    CalcStrHashSumOverLength,
    CalcStrHashCRC32
};
const int N_HASH_FUNCS = sizearr (HASH_FUNCS);

const char * const TESTS_NAMES[] =
{
    "return zero",
    "ascii first",
    "word length",
    "control sum",
    "control sum over length",
    "crc32"
};

const char * const ANALYZE_HF_RES_PATHS[] =
{
    "hash_table/analysis/results/csv/retzero_hf.csv",
    "hash_table/analysis/results/csv/ascii_first_hf.csv",
    "hash_table/analysis/results/csv/wd_len_hf.csv",
    "hash_table/analysis/results/csv/ctrl_sum_hf.csv",
    "hash_table/analysis/results/csv/sum_by_len_hf.csv",
    "hash_table/analysis/results/csv/crc32_hf.csv"
};

const char * const DFLT_PREPROCESSED_TARGET_DATA_PATH
                                             = "texts/preprocessed/GeorgeRRMartin.txt";
const char * const DFLT_TEST_RESULTS_PATH    = "hash_table/analysis/results/performance/res.csv";
const char * const DFLT_TEST_CASES_PATH      = "texts/preprocessed/words_ready.txt";
// const char * const DFLT_TEST_CASES_PATH      = "texts/preprocessed/small.txt";
const int DFLT_HASH_TABLE_SIZE               = 1001;

int main (int argc, const char **argv)
{
    if (argc == 1) printf ("Please, compile with --help to see options\n");

    argv++; // skip prog name

    while (argc > 1)
    {
        if (streq (*argv, "--help"))
        {
            printf ("-ahf | analyze hash functions\n"
                    "-tpf | test performance of multiple hash functions\n");

            argc--;
        }
        else if (streq (*argv, "-ahf") || streq (*argv, "--analyze-hash-functions"))
        {
            const char *preprocessed_target_data_path = argv[1];
            FILE *preprocessed_target_data_file = fopen (preprocessed_target_data_path, "rb");
            if (!preprocessed_target_data_file)
            {
                ERROR ("Couldn't open \"%s\" as target data file. Loading default file instead: \"%s\"...", preprocessed_target_data_path, DFLT_PREPROCESSED_TARGET_DATA_PATH);
                preprocessed_target_data_file = fopen (DFLT_PREPROCESSED_TARGET_DATA_PATH, "rb");
                if (!preprocessed_target_data_file)
                {
                    ERROR ("Couldn't open default file \"%s\"...\nPlease, try again!", DFLT_PREPROCESSED_TARGET_DATA_PATH);
                    return 1;
                }
            }

            AnalyzeMultipleHashFunctions (ANALYZE_HF_RES_PATHS,
                                          preprocessed_target_data_file,
                                          HASH_FUNCS,
                                          N_HASH_FUNCS,
                                          DFLT_HASH_TABLE_SIZE);

            fclose (preprocessed_target_data_file);

            argv++; // skip preprocessed target data path
        }
        else if (streq (*argv, "-tpf") || streq (*argv, "--test-performance"))
        {
            const char *test_results_path = argv[1];
            FILE *test_results_file = fopen (test_results_path, "wb");
            if (!test_results_file)
            {
                ERROR ("Couldn't open \"%s\" as test results file. Loading default file instead: \"%s\"...", test_results_path, DFLT_TEST_RESULTS_PATH);
                test_results_file = fopen (DFLT_TEST_RESULTS_PATH, "rb");
                if (!test_results_file)
                {
                    ERROR ("Couldn't open default file \"%s\"...\nPlease, try again!", DFLT_TEST_RESULTS_PATH);
                    return 1;
                }
            }
            else
            {
                argc--;
            }

            const char *test_cases_path = argv[2];
            FILE *test_cases_file = fopen (test_cases_path, "rb");
            if (!test_cases_file)
            {
                ERROR ("Couldn't open \"%s\" as test cases file. Loading default file instead: \"%s\"...", test_results_path, DFLT_TEST_CASES_PATH);
                test_cases_file = fopen (DFLT_TEST_CASES_PATH, "rb");
                if (!test_cases_file)
                {
                    ERROR ("Couldn't open default file \"%s\"...\nPlease, try again!", DFLT_TEST_CASES_PATH);
                    return 1;
                }
            }
            else
            {
                argc--;
            }

            const char *preprocessed_target_data_path = argv[3];
            FILE *preprocessed_target_data_file = fopen (preprocessed_target_data_path, "rb");
            if (!preprocessed_target_data_file)
            {
                ERROR ("Couldn't open \"%s\" as target data file. Loading default file instead: \"%s\"...", preprocessed_target_data_path, DFLT_PREPROCESSED_TARGET_DATA_PATH);
                preprocessed_target_data_file = fopen (DFLT_PREPROCESSED_TARGET_DATA_PATH, "rb");
                if (!preprocessed_target_data_file)
                {
                    ERROR ("Couldn't open default file \"%s\"...\nPlease, try again!", DFLT_PREPROCESSED_TARGET_DATA_PATH);
                    return 1;
                }
            }

            TestPerformanceMultipleHashFunctions (test_results_file,
                                                  test_cases_file,
                                                  TESTS_NAMES,
                                                  preprocessed_target_data_file,
                                                  HASH_FUNCS,
                                                  N_HASH_FUNCS,
                                                  DFLT_HASH_TABLE_SIZE);


            fclose (preprocessed_target_data_file);
            fclose (test_results_file);

            argv += 3;
        }

        argc--;
    }

    return 0;
}