#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include <stdio.h>
#include "../super_list.h"

typedef enum {
    LST_ERR_NO_LIST_PTR    = 1,
    LST_ERR_NO_DATA_PTR    = 2,
    LST_ERR_NO_NEXT_PTR    = 4,
    LST_ERR_NO_PREV_PTR    = 8,
    LST_ERR_HEAD_TAIL      = 16,
    LST_ERR_CHAIN          = 32,
    LST_ERR_FRE_PREV       = 64,
    LST_ERR_FRE            = 128,
    LST_ERR_SIZE           = 256,
} ERRORS_LIST;

const char * const DUMP_FNAME = "list_dump.svg";
const int STYLE_TAG_SIZE = 1000;

int    ListDump       (const char * fname, const List * list, size_t err_vec, ListDebugInfo debug_info);
char * CreateVals     (const List * list, size_t size);
char * CreateNodes    (const List * list, size_t size);
char * CreateEdges    (const List * list, size_t size);
char * CreateDotCode  (const List * list);
int    WriteDotCode   (const char * fname, const char * dot_code);

char * CreateAddInfo  (size_t err_vec, ListDebugInfo debug_info);

int WriteHTML         (const char * HTML_fname, int dump_id, size_t err_vec, ListDebugInfo debug_info);

#endif // LIST_DUMP_H
