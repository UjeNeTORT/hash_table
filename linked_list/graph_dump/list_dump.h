#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../super_list.h"

const int          DEFAULT_BUF_SIZE = 1000;
const char * const DUMP_FNAME       = "list_dump.svg";
const int          STYLE_TAG_SIZE   = 1000;

struct List;
struct ListDebugInfo;

typedef enum {
    LST_ERR_NO_LIST_PTR    = 1 << 0,
    LST_ERR_NO_DATA_PTR    = 1 << 1,
    LST_ERR_NO_NEXT_PTR    = 1 << 2,
    LST_ERR_NO_PREV_PTR    = 1 << 3,
    LST_ERR_HEAD_TAIL      = 1 << 4,
    LST_ERR_CHAIN          = 1 << 5,
    LST_ERR_FRE_PREV       = 1 << 6,
    LST_ERR_FRE            = 1 << 7,
    LST_ERR_SIZE           = 1 << 8,
} ERRORS_LIST;

int    ListDump       (const char * fname, List * list, size_t err_vec, ListDebugInfo debug_info);
char * CreateVals     (const List * list, size_t size);
char * CreateNodes    (const List * list, size_t size);
char * CreateEdges    (const List * list, size_t size);
char * CreateDotCode  (const List * list);
int    WriteDotCode   (const char * fname, const char * dot_code);

char * CreateAddInfo  (size_t err_vec, ListDebugInfo debug_info);

int WriteHTML         (const char * HTML_fname, int dump_id, size_t err_vec, ListDebugInfo debug_info);

#endif // LIST_DUMP_H
