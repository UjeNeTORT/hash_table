#include "hash_functions.h"

volatile u_int64_t  CalcStrHashReturnZero    (const char * const word, size_t word_length)
{
    assert (word);

    return 0;
}

volatile u_int64_t  CalcStrHashFirstLetter   (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashWordLength    (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashControlSum    (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashSumOverLength (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashDedFormula    (const char * const word, size_t word_length);

volatile u_int64_t  CalcStrHashCRC32         (const char * const word, size_t word_length);