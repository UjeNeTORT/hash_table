#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <assert.h>
#include <stdlib.h>

#include "../../config.h"

volatile hash_t  CalcStrHashReturnZero    (ht_key_t key);

volatile hash_t  CalcStrHashFirstLetter   (ht_key_t key);
volatile hash_t  CalcStrHashWordLength    (ht_key_t key);
volatile hash_t  CalcStrHashControlSum    (ht_key_t key);
volatile hash_t  CalcStrHashSumOverLength (ht_key_t key);
volatile hash_t  CalcStrHashDedFormula    (ht_key_t key);

volatile hash_t  CalcStrHashCRC32         (ht_key_t key);

#endif // HASH_FUNCTIONS_H