#ifndef SUPER_LIST_H
#define SUPER_LIST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "../common/common.h"
#include "graph_dump/list_dump.h"

const char * const DOT_DUMP_FILENAME = "graph.dot";
const char * const HTML_DUMP_FNAME   = "linked_list/graph_dump/dumps/dump1.html";

#define LIST_DEBUG

#ifdef LIST_DEBUG
    #define ON_DEBUG(...) __VA_ARGS__;
#else
    #define ON_DEBUG(...)
#endif // DEBUG

#ifdef LIST_DEBUG

    #define DEBUG_INFO(list) {#list, __FILE__, __FUNCTION__, __LINE__}

#else

    #define DEBUG_INFO(list) {NULL, NULL, NULL, -1}

#endif

/**
 * @brief abort program and calls dump if list is corrupted, else do nothing
 *
 * @param list list to get checked
 *
 * @warning abort inside
*/
#define VERIFY_LIST(list, debug_info)                                \
{                                                                    \
    size_t err_vec = ListVerifier((const List *) list);              \
    if (err_vec != 0)                                                \
    {                                                                \
        ERROR ("List %s called from %s (%d) is corrupted (%#b), aborting...\n", debug_info.list_name, debug_info.filename, debug_info.line, err_vec); \
        ListDump(DOT_DUMP_FILENAME, list, err_vec, debug_info);      \
        ListDtor(list);                                              \
        abort();                                                     \
    }                                                                \
}

/**
 * @brief abort if id not allowed for list, otherwise do nothing
 *
 * @param list list
 * @param id   id to get checked
 *
 * @warning abort inside
*/
#define VERIFY_ID(list, id, debug_info)     \
{                                           \
    if (ListVerifyId(list, id, debug_info)) \
        ABORT_LIST(list, -1, debug_info);   \
}                                           \

/**
 * @brief smooth abort, calls dump inside and destroys list
 *
 * @param list    list
 * @param err_vec vector of list errors code (if == -1 - aborted without collecting info about errors)
*/
#define ABORT_LIST(list, err_vec, debug_info)                     \
{                                                                 \
    ListDump(DOT_DUMP_FILENAME, (list), (err_vec), (debug_info)); \
    ListDtor(list);                                               \
    assert (0);                                                   \
}

typedef enum
{
    CPY_NO_ERR     = 0, //< successfully copied
    CPY_ERR_MEMCPY = 1, //< memcpy returned nullptr
} ListCopyRes;

typedef enum
{
    REALLC_NO_ERR    = 0, // successfully realloced
    REALLC_FORBIDDEN = 1, // reallocation didnt happen (in case new_size <= size - such functionality is not provided)
    REALLC_ERR       = 2, // reallocation started but finished with error (realloc returned nullptr or func designed to treat such new_size as an error)
} ListReallocRes;

/**
 * @warning access allowed only by functions ListDoSmth
 *
 * @note list node and element are synonyms. Element can be free or it can store some value of type list_elem_t
*/
struct List
{
    list_elem_t * data;  // by default stores LIST_POISON
    int * next;     // contains ids of next elements for each elem. There are two separate singly-linked lists in "next" (one of them is for free nodes - it statrs at id "fre").
    int * prev;     // contains ids of prev elements for each non-free elem. If node is free, contains -1;

    int fre;        // id of first free node in "next" array

    int size;       // list size, can be changed via ListRealloc
};

struct ListDebugInfo
{
    const char * list_name;
    const char * filename;
    const char * funcname;
    int          line;
};

/**
 * todo docs
*/
int ListPrintfErrCorruptedList(ListDebugInfo debug_info);

/**
 * todo docs
*/
size_t ListVerifier (const List * list);

/**
 * @brief check if list id "id" is "addressable"
 *
 * @param list list
 * @param id   id to get checked
 *
 * @return 0 if id is "addressable" for list
 * @return 1 if id is wrong (< 0 or >= size)
 *
 * @example if we pass id = -1 (ListInsertAfter(list, -1 , 111)) ListVerifyId returns -> 1 (error code)
 *
 * @note this func is called every time inside functions to which we pass id.
 * @warning it is usually called inside VERIFY_ID macros, which has abort inside
*/
int ListVerifyId  (List * list, int id, ListDebugInfo debug_info);
#define VerifyIdList(list, id) ListVerifyId((list), (id), DEBUG_INFO(list))

/**
 * @brief create new list of size "size"
 *
 * @param size desired size of list
 *
 * @return new list pointer
 *
 * @note after construction every element stores "LIST_POISON" and marked as free.
 * fre points to the first element, prev of each element is -1, next points to next free element
*/
List *ListCtor (int size);

/**
 * @brief destroy list (free all the allocated memory)
 *
 * @param list list
 *
 * @return 0
*/
int ListDtor (List * list);

/**
 * @brief copy everything from list_src to list_dst
 *
 * @details container for list_dst is to be created manually outside the function
 *
 * @param list_dst destination list to which we copy
 * @param list_src source list from which we copy
 *
 * @return ListCopyRes (0 - if ok, not 0 - if there were errors inside)
*/
ListCopyRes ListCopy (List * list_dst, const List * list_src, ListDebugInfo debug_info);
#define CopyList(list_dst, list_src) ListCopy(list_dst, list_src, DEBUG_INFO(list))

/**
 * @brief reallocate list (can only increase its size)
 *
 * @param list     list
 * @param new_size desired size of list
 *
 * @return REALLC_NO_ERR    = 0 if reallocation have happened
 * @return REALLC_FORBIDDEN = 1 if reallocation didnt happen (highly likely due to passing new_size <= size to function - such functionality is not provided)
 * @return REALLC_ERR       = 2 if reallocation started but couldnt finish due to an error
 *
 * @note if "LINEARIZATION" is on, it will be called inside this func every time before reallocation
*/
ListReallocRes ListRealloc (List * list, int new_size, ListDebugInfo debug_info);
#define ReallocList(list, new_size) ListRealloc(list, new_size, DEBUG_INFO(list))

/**
 *! @warning NOT FINISHED, NOT OPTIMAL, SLOW, DONT USE, CREATES NEW LIST INSIDE!!!
*/
int ListMakeLinear (List * list); //! not finished

/**
 * @brief get value stored in list->data[id]
 *
 * @param list list
 * @param id   index of val
 *
 * @return value stored in list->data[id]
 * @return LIST_POISON if "id" stores nothing or "id" is invalid
*/
#define GetElemListId(list, id) ListIdGetElem(list, id, DEBUG_INFO(list))
list_elem_t ListIdGetElem (List * list, int id, ListDebugInfo debug_info);

/**
 *
 * @brief traverse through the list and find "val". If found return id of "val", else return -1;
 *
 * @param list list
 * @param val  value, id of which is to be returned
 *
 * @return id of "val" in the list if found
 * @return -1 if "val" not found in the list
 *
 *
 * @warning traverses through the list. Much slower than access by id
*/
#define GetIdListKey(list, key) ListKeyGetId(list, key, DEBUG_INFO(list))
int ListKeyGetId (List * list, ht_key_t key, ListDebugInfo debug_info);

/**
 * @brief insert "val" in the beginning of the list
 *
 * @param list list
 * @param val  value which is to be inserted in the beginning of list
 *
 * @return id of "val" in the list
 * @return -1 if no free place in the list (no more reallocs we can do)
*/
#define InsertBeginList(list, key) ListInsertBegin(list, key, DEBUG_INFO(list))
int ListInsertBegin (List * list, ht_key_t key, ListDebugInfo debug_info);

/**
 * @brief insert "val" in the end of the list
 *
 * @param list list
 * @param val  value which is to be inserted in the end of the list
 *
 * @return id of "val" in the list
 * @return -1 if no free place in the list (no more reallocs we can do)
*/
#define InsertEndList(list, key) ListInsertEnd(list, key, DEBUG_INFO(list))
int ListInsertEnd (List * list, ht_key_t key, ListDebugInfo debug_info);

/**
 * @brief insert "val" after value with id = "id"
 *
 * @param list list
 * @param id   id of element after which  "val" is to be inserted
 * @param val  value which is to be inserted after "id"
 *
 * @return id of new element "val" in the list
 * @return -1 if no free space in the list (no more reallocs we can do)
*/
#define InsertAfterIdList(list, id, key) ListInsertAfterId(list, id, key, DEBUG_INFO(list))
int ListInsertAfterId (List * list, int id, ht_key_t key, ListDebugInfo debug_info);

/**
 * @brief insert "val" before value with id = "id"
 *
 * @param list list
 * @param id   id of element before which  "val" is to be inserted
 * @param val  value which is to be inserted before "id"
 *
 * @return id of new element "val" in the list
 * @return -1 if no free space in the list (no more reallocs we can do)
*/
#define InsertBeforeList(list, id, key) ListInsertBefore(list, id, key, DEBUG_INFO(list))
int ListInsertBefore (List * list, int id, ht_key_t key, ListDebugInfo debug_info);

/**
 * @brief delete and return value with id = "id".
 *
 * @param list list
 * @param val  value to be deleted and id of which is to be returned (if found)
 *
 * @return id of "val" in the list if found
 * @return -1 if "val" not found in the list
*/
list_elem_t ListIdDelete    (List * list, int id, ListDebugInfo debug_info);
#define DeleteIdList(list, id) ListIdDelete(list, id, DEBUG_INFO(list))

/**
 * @brief iterate through list and find "val", if found - delete and return its id, else - return -1
 *
 * @param list list
 * @param val  value to be deleted and id of which is to be returned
 *
 * @return former id of deleted "val" if val found
 * @return -1 if "val" not found in the list
 *
*/
#define DeleteKeyList(list, key) ListKeyDelete(list, key, DEBUG_INFO(list))
int ListKeyDelete (List * list, ht_key_t key, ListDebugInfo debug_info);

int IsListElemEq (const list_elem_t el_1, const list_elem_t el_2);

int AssignListEl (list_elem_t *el_dst, const list_elem_t *el_src);

int IncreaseValListId (List *list, int id);

#endif // SUPER_LIST_H
