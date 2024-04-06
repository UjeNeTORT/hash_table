#include "hash_functions.h"

volatile hash_t  CalcStrHashReturnZero    (ht_key_t key)
{
    assert (key);

    return 0;
}

volatile hash_t  CalcStrHashFirstLetter   (ht_key_t key)
{
    return ((const char *) key)[0];
}

volatile hash_t  CalcStrHashWordLength    (ht_key_t key)
{
    return strlen ((const char *) key);

}

volatile hash_t  CalcStrHashControlSum    (ht_key_t key)
{
    hash_t sum = 0;

    while (*((const char *) key))
    {
        sum += *((const char *) key);
        key++;
    }

    return sum;
}

volatile hash_t  CalcStrHashSumOverLength (ht_key_t key)
{
    hash_t sum = 0;

    ht_key_t key_init = key;

    while (*((const char *) key))
    {
        sum += *((const char *) key);
        key++;
    }

    return sum / strlen ((const char *) key_init);
}

volatile hash_t  CalcStrHashDedFormula    (ht_key_t key)
{
    // what the fuck i cant read my conspect
}

volatile hash_t  CalcStrHashCRC32         (ht_key_t key)
{
    return crc32 (0, (const unsigned char *) key, strlen ((const char *) key));
}