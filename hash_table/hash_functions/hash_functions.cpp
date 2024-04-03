#include "hash_functions.h"

volatile hash_t  CalcStrHashReturnZero    (ht_key_t key)
{
    assert (key);

    return 0;
}

volatile hash_t  CalcStrHashFirstLetter   (ht_key_t key);
volatile hash_t  CalcStrHashWordLength    (ht_key_t key);
volatile hash_t  CalcStrHashControlSum    (ht_key_t key);
volatile hash_t  CalcStrHashSumOverLength (ht_key_t key);
volatile hash_t  CalcStrHashDedFormula    (ht_key_t key);

volatile hash_t  CalcStrHashCRC32         (ht_key_t key);