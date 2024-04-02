#include <stdio.h>

#include "hash_table/hash_table.h"
#include "hash_table/hash_functions/hash_functions.h"

int main ()
{
    hash_func_ptr hash_func = CalcStrHashReturnZero;

    HashTable *first_ht = HashTableCtor (10, hash_func);

    HashTableDtor (first_ht);

    return 0;
}