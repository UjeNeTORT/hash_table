#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <assert.h>
#include <immintrin.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "../../config.h"

hash_t  CalcStrHashReturnZero    (ht_key_t key);

hash_t  CalcStrHashFirstLetter   (ht_key_t key);
hash_t  CalcStrHashWordLength    (ht_key_t key);
hash_t  CalcStrHashControlSum    (ht_key_t key);
hash_t  CalcStrHashSumOverLength (ht_key_t key);
hash_t  CalcStrHashDedFormula    (ht_key_t key);

hash_t  CalcStrHashCRC32         (ht_key_t key);

#endif // HASH_FUNCTIONS_H