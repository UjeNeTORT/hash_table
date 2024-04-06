#include <stdio.h>

#include "hash_table/hash_table.h"
#include "hash_table/hash_functions/hash_functions.h"

int GetFileSize (FILE *file);

int main ()
{
    hash_func_ptr_t hash_func = CalcStrHashReturnZero;

    HashTable *first_ht = HashTableCtor (10, hash_func);

    FILE *fin = fopen ("texts/preprocessed/small.txt", "rb");

    int fsize = GetFileSize(fin);
    char *buf = (char *) calloc (fsize + 1, sizeof (char));
    fread (buf, sizeof (char), fsize, fin);
    fclose (fin);
    HashTableLoadTargetData (first_ht, buf, '\n');

    HashTableDtor (first_ht);
    free (buf);

    return 0;
}

int GetFileSize (FILE *file)
{
    if (!file) return 0;

    int fsize = 0;

    fseek (file, 0, SEEK_END);
    fsize = ftell(file);
    rewind (file);

    return fsize;
}