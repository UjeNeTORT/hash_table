#include <stdio.h>

#include "hash_table/hash_table.h"
#include "hash_table/hash_functions/hash_functions.h"
#include "hash_table/analysis/hash_functions_analysis.h"

int main ()
{
    FILE *fin = fopen ("texts/preprocessed/GeorgeRRMartin.txt", "rb");
    int fsize = GetFileSize(fin);
    printf ("%d\n", fsize);
    char *buf1 = (char *) calloc (fsize + 1, sizeof (char));
    char *buf2 = (char *) calloc (fsize + 1, sizeof (char));
    char *buf3 = (char *) calloc (fsize + 1, sizeof (char));
    char *buf4 = (char *) calloc (fsize + 1, sizeof (char));
    char *buf5 = (char *) calloc (fsize + 1, sizeof (char));
    char *buf6 = (char *) calloc (fsize + 1, sizeof (char));
    fread (buf1, sizeof (char), fsize, fin);
    strcpy (buf2, buf1);
    strcpy (buf3, buf1);
    strcpy (buf4, buf1);
    strcpy (buf5, buf1);
    strcpy (buf6, buf1);

    fclose (fin);

    hash_func_ptr_t hash_func = CalcStrHashReturnZero;
    AnalyzeHashFunction ("hash_table/analysis/results/retzero_hf.csv", buf1, hash_func, 1001);
    PRINTF_DEBUG ("1");
    hash_func = CalcStrHashFirstLetter;
    AnalyzeHashFunction ("hash_table/analysis/results/ascii_first_hf.csv", buf2, hash_func, 1001);
    PRINTF_DEBUG ("2");
    hash_func = CalcStrHashWordLength;
    AnalyzeHashFunction ("hash_table/analysis/results/wd_len_hf.csv", buf3, hash_func, 1001);
    PRINTF_DEBUG ("3");
    hash_func = CalcStrHashControlSum;
    AnalyzeHashFunction ("hash_table/analysis/results/ctrl_sum_hf.csv", buf4, hash_func, 1001);
    PRINTF_DEBUG ("4");
    hash_func = CalcStrHashSumOverLength;
    AnalyzeHashFunction ("hash_table/analysis/results/sum_by_len_hf.csv", buf5, hash_func, 1001);
    PRINTF_DEBUG ("5");
    hash_func = CalcStrHashCRC32;
    AnalyzeHashFunction ("hash_table/analysis/results/crc32_hf.csv", buf6, hash_func, 1001);

    free (buf1);
    free (buf2);
    free (buf3);
    free (buf4);
    free (buf5);

    return 0;
}