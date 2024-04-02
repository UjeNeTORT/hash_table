#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <assert.h>
#include <stdlib.h>

typedef volatile u_int64_t (*hash_func_ptr)(const char *word, size_t word_length);

volatile u_int64_t  CalcStrHashReturnZero    (const char * const word, size_t word_length);

volatile u_int64_t  CalcStrHashFirstLetter   (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashWordLength    (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashControlSum    (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashSumOverLength (const char * const word, size_t word_length);
volatile u_int64_t  CalcStrHashDedFormula    (const char * const word, size_t word_length);

volatile u_int64_t  CalcStrHashCRC32         (const char * const word, size_t word_length);

#endif // HASH_FUNCTIONS_H