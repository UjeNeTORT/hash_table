#include "super_list.h"

// DSL
#define NEXT(index) list->next[(index)]
#define PREV(index) list->prev[(index)]
#define DATA(index) list->data[(index)]

/**
 * @brief increase size of list from size to new_size
 *
 * @param list     list
 * @param new_size desired new size of the list
 *
 * @return REALLC_NO_ERR    = 0 if reallocation have happened
 * @return REALLC_ERR       = 2 if reallocation started but couldnt finish due to an error
 *
 * @note function is designed to increase size of the list so if new_size < size it would be a mistake and fucntion
 *       will return an error code
*/
static ListReallocRes  ListReallocUp (List * list, int new_size, ListDebugInfo debug_info);

List *ListCtor (int size)
{
    if (size <= 0)
    {
        fprintf(stderr, "ERROR ListCtor: size (%d) must be more than 0\n", size);

        return {};
    }

    List    *list = (List *)   calloc (1, sizeof (List));
    elem_t  *data = (elem_t *) calloc (size, sizeof(elem_t));
    int     *next = (int *)    calloc (size, sizeof(int));
    int     *prev = (int *)    calloc (size, sizeof(int));


    list->data = data;
    list->next = next;
    list->prev = prev;

    list->fre  = 1;

    list->size = size;

    data[0] = POISON;
    next[0] = 0; // head
    prev[0] = 0; // tail

    for (int i = 1; i < size; i++)
    {
        data[i] = POISON;
        next[i] = i + 1;
        prev[i] = -1;
    }

    next[size - 1] = -1; // no next node after the last one

    return list;
}

int ListDtor (List * list)
{
    assert(list);

    free (list->data);

    free (list->next);

    free (list->prev);

    free (list);

    return 0;
}

ListCopyRes ListCopy (List * list_dst, List * list_src, ListDebugInfo debug_info)
{
    VERIFY_LIST(list_src, debug_info);

    if (!memcpy(list_dst->data, list_src->data, list_src->size * sizeof(elem_t)))
    {
        return CPY_ERR_MEMCPY;
    }

    if (!memcpy(list_dst->next, list_src->next, list_src->size * sizeof(int)))
    {
        return CPY_ERR_MEMCPY;
    }

    if (!memcpy(list_dst->prev, list_src->prev, list_src->size * sizeof(int)))
    {
        return CPY_ERR_MEMCPY;
    }

    list_dst->fre = list_src->fre;

    return CPY_NO_ERR;
}

ListReallocRes ListRealloc  (List * list, int new_size, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    ListReallocRes ret_val = REALLC_NO_ERR;

    #ifdef LINEARIZATION
        ListMakeLinear(list);
    #endif // LINEARIZATION

    if (new_size > list->size)
    {
        ret_val = ListReallocUp(list, new_size, debug_info);
    }
    else if (new_size < list->size)
    {
        ret_val = REALLC_FORBIDDEN;
    }
    else if (new_size == list->size)
    {
        ret_val = REALLC_FORBIDDEN;
    }

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return ret_val;
}

static ListReallocRes
ListReallocUp (List * list, int new_size, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    if (new_size <= list->size)
    {
        return REALLC_ERR;
    }

    list->data = (elem_t *) realloc(list->data, new_size * sizeof(elem_t));
    if (!list->data)
    {
        return REALLC_ERR;
    }

    list->next = (int *) realloc(list->next, new_size * sizeof(int));
    if (!list->next)
    {
        return REALLC_ERR;
    }

    list->prev = (int *) realloc(list->prev, new_size * sizeof(int));
    if (!list->prev)
    {
        return REALLC_ERR;
    }

    int id_free = list->fre;

    while (NEXT(id_free) != -1)
    {
        id_free = NEXT(id_free);
    }

    for (int i = id_free; i < new_size; i++)
    {
        NEXT(i) = i + 1;
        DATA(i) = POISON;
        PREV(i) = -1;
    }

    NEXT(new_size - 1) = -1; // last element has no next

    list->size = new_size;

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return REALLC_NO_ERR;
}

elem_t ListIdFind (List * list, int id, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    elem_t val = POISON;

    if (0 < id && id < list->size)
        val = DATA(id);
    else
        fprintf(stderr, "ListIdFind: invalid id %d\n", id);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return val;
}

int MegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToListValFind (List * list, elem_t val, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int id = -1; // not found

    for (int i = NEXT(0); i != 0; i = NEXT(i))
    {
        if (DATA(i) == val)
        {
            id = i;
            break;
        }
    }

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return id;
}

int ListInsertBegin (List * list, elem_t val, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int new_id = InsertAfterList(list, 0, val);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id;  // where inserted value is
}

int ListInsertEnd (List * list, elem_t val, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int new_id = InsertAfterList(list, PREV(0), val);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id;  // where inserted value is
}

int ListInsertAfter (List * list, int id, elem_t val, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);
    VERIFY_ID(list, id, debug_info);

    if (list->fre == -1)
    {
        // if no room left - abort
        fprintf(stderr, "ListInsertAfter: no room left for insertion\n");
        ABORT_LIST(list, -1, debug_info);

        ReallocList (list, list->size + 1); // NOT OPTIMAL!
    }

    int new_id = list->fre;
    list->fre = NEXT(new_id);

    DATA(new_id) = val;

    int old_nxt = NEXT(id);

    NEXT(id) = new_id;
    NEXT(new_id) = old_nxt;

    PREV(new_id) = PREV(old_nxt);
    PREV(old_nxt) = new_id;

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id; // where inserted value is
}

int ListInsertBefore (List * list, int id, elem_t val, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);
    VERIFY_ID(list, id, debug_info);

    int new_id = InsertAfterList(list, PREV(id), val);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id;
}

elem_t ListIdDelete (List * list, int id, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);
    VERIFY_ID(list, id, debug_info);

    int prev = PREV(id);
    int next = NEXT(id);

    NEXT(prev) = next;
    PREV(next) = prev;

    elem_t deleted_el = DATA(id);
    DATA(id) = POISON;

    PREV(id) = -1;
    NEXT(id) = list->fre;
    list->fre = id;

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return deleted_el;
}

int ListValDelete (List * list, elem_t val, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int id = FindValMegaSuperSlowTenLoopsTwentyDrunkenEngineersTryingToList(list, val);

    if (id != -1)
    {
        DeleteIdList(list, id);
    }

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return id;
}

size_t ListVerifier (const List * list)
{
    size_t err_vec = 0;

    if (!list)
    {
        err_vec |= LST_ERR_NO_LIST_PTR;

        return err_vec;
    }

    if (!list->data)
    {
        err_vec |= LST_ERR_NO_DATA_PTR;

        return err_vec;
    }

    if (!list->next)
    {
        err_vec |= LST_ERR_NO_NEXT_PTR;

        return err_vec;
    }

    if (!list->prev)
    {
        err_vec |= LST_ERR_NO_PREV_PTR;

        return err_vec;
    }

    if (list->size <= 0)
    {
        err_vec |= LST_ERR_SIZE;
    }

    if (NEXT(0) < 0 || PREV(0) > list->size)
    {
        err_vec |= LST_ERR_HEAD_TAIL;
    }

    // check if nexts of used elements form chain which ends at tail
    for (int i = NEXT(0), cnt = 0; i != PREV(0); i = NEXT(i), cnt++)
    {
        if (cnt > list->size)
        {
            err_vec |= LST_ERR_CHAIN;
            break;
        }

        if (PREV(i) == -1)
        {
            err_vec |= LST_ERR_FRE_PREV;
        }
    }

    if (list->fre <= 0 || list->fre > list->size)
    {
        if (list->fre == -1)
        {
            return err_vec;
        }
        else
        {
            err_vec |= LST_ERR_FRE;

            return err_vec; // because further we adress fre, which is incorrect
        }
    }

    // check if nexts of free elements form chain and that their prevs = -1
    for (int i = NEXT(list->fre), cnt = 0; i != -1; i = NEXT(i), cnt++)
    {
        if (cnt > list->size || i >= list->size)
        {
            err_vec |= LST_ERR_CHAIN;

            break;
        }

        if (PREV(i) != -1)
        {
            err_vec |= LST_ERR_FRE_PREV;
        }
    }

    return err_vec;
}

int ListVerifyId (List * list, int id, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    if (id < 0)
    {
        fprintf(stderr, "List id %d < 0\n", id);

        return 1;
    }

    if (id >= list->size)
    {
        fprintf(stderr, "List id %d >= list size\n", id);

        return 1;
    }

    return 0;
}

int ListPrintfErrCorruptedList(ListDebugInfo debug_info)
{
    return fprintf(stderr, "ERROR List %s called from %s (%d) is corrupted, aborting...\n",
                                debug_info.list_name, debug_info.filename, debug_info.line);
}
