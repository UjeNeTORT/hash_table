#include "hash_functions.h"

hash_t CalcStrHashReturnZero    (ht_key_t key)
{
    assert (key);

    return 0;
}

hash_t CalcStrHashFirstLetter   (ht_key_t key)
{
    return ((const char *) key)[0];
}

hash_t CalcStrHashWordLength    (ht_key_t key)
{
    return strlen ((const char *) key);

}

hash_t CalcStrHashControlSum    (ht_key_t key)
{
    hash_t sum = 0;

    while (*((const char *) key))
    {
        sum += *((const char *) key);
        key++;
    }

    return sum;
}

hash_t CalcStrHashSumOverLength (ht_key_t key)
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

hash_t CalcStrHashROL    (ht_key_t key)
{
    hash_t hash = 0;
    u_int8_t i = 0;

    while (key[i])
    {
        hash = ((hash << 1) | (hash >> 63));
        hash = hash ^ key[i];
        i++;
    }

    return hash;
}

hash_t CalcStrHashCRC32 (ht_key_t key)
{
    // hash_t volatile hash = crc32 (0, (const unsigned char *) key, (u_int32_t) (key)[0]);

    volatile hash_t hash = 0;

    asm volatile (
        ".intel_syntax noprefix\n\t"
        "xor rax, rax\n\t"
        "mov rcx, 4\n"
        ".loop:\n\t"
        "crc32 rax, QWORD PTR [rdi]\n\t"
        "add rdi, 8\n\t"
        "dec rcx\n\t"
        "cmp rcx, 0\n\t"
        "jne .loop\n\t"
        "mov %0, rax\n\t"
        ".att_syntax prefix\n\t"
        : "=r" (hash)
        :
        : "rax", "rcx", "rdi"
    );

    // hash = _mm_crc32_u64 (hash, ((int64_t *)key)[0]);
    // hash = _mm_crc32_u64 (hash, ((int64_t *)key)[1]);
    // hash = _mm_crc32_u64 (hash, ((int64_t *)key)[2]);
    // hash = _mm_crc32_u64 (hash, ((int64_t *)key)[3]);

    return hash;
}