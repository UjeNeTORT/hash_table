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

List *ListCtor (int list_capacity)
{
    if (list_capacity <= 0)
    {
        ERROR ("capacity (%d) must be more than 0\n", list_capacity);

        return NULL;
    }

    int size = list_capacity + 1;  // add one fictitious element

    List        *list = (List *)        calloc (1, sizeof (List));
    list_elem_t *data = (list_elem_t *) calloc (size, sizeof(list_elem_t));
    int         *next = (int *)         calloc (size, sizeof(int));
    int         *prev = (int *)         calloc (size, sizeof(int));

    list->data = data;
    list->next = next;
    list->prev = prev;

    list->fre  = 1;

    list->size = size;

    AssignListEl (&data[0], &LIST_POISON);
    next[0] = 0; // head
    prev[0] = 0; // tail

    for (int i = 1; i < size; i++)
    {
        AssignListEl (&data[i], &LIST_POISON);
        next[i] = i + 1;
        prev[i] = -1;
    }

    return list;
}

int ListDtor (List * list)
{
    if (!list) return 0;

    free (list->data);

    free (list->next);

    free (list->prev);

    free (list);

    return 0;
}

ListCopyRes ListCopy (List * list_dst, List * list_src, ListDebugInfo debug_info)
{
    VERIFY_LIST(list_src, debug_info);

    if (!memcpy(list_dst->data, list_src->data, list_src->size * sizeof(list_elem_t)))
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

    if (new_size <= list->size) return REALLC_ERR;

    list->data = (list_elem_t *) realloc(list->data, new_size * sizeof(list_elem_t));
    if (!list->data) return REALLC_ERR;

    list->next = (int *) realloc(list->next, new_size * sizeof(int));
    if (!list->next) return REALLC_ERR;

    list->prev = (int *) realloc(list->prev, new_size * sizeof(int));
    if (!list->prev) return REALLC_ERR;

    int id_free = list->fre;

    for (int i = id_free; i < new_size; i++)
    {
        NEXT(i) = i + 1;
        AssignListEl (&DATA(i), &LIST_POISON);
        PREV(i) = -1;
    }

    NEXT(new_size - 1) = new_size; // last element has no next

    list->size = new_size;

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return REALLC_NO_ERR;
}

list_elem_t ListIdGetElem (List * list, int id, ListDebugInfo debug_info)
{
    assert (0 < id && id <= list->size);
    VERIFY_LIST(list, debug_info);

    list_elem_t l_elem = LIST_POISON;

    AssignListEl (&l_elem, &DATA(id));

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return l_elem;
}

int ListKeyGetId (List * list, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int id = -1; // not found

    for (int i = NEXT(0); i != 0; i = NEXT(i))
        if (streq (DATA(i).key, key))
        {
            id = i;
            break;
        }

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return id;
}

int ListKeyGetValSorted (List * list, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int value = LIST_POISON.value;
    size_t curr_id = NEXT (0);

    while (strcmp (DATA (curr_id).key, key) < 0)
    {
        if (curr_id == PREV (0)) goto verify_and_return;

        curr_id = NEXT (curr_id);
    }

    if (strcmp (DATA (curr_id).key, key) == 0) value = DATA (curr_id).value;

verify_and_return:
    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return value;
}


int ListInsertBegin (List * list, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int new_id = InsertAfterIdList(list, 0, key);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id;  // where inserted value is
}

int ListInsertEnd (List * list, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int new_id = InsertAfterIdList(list, PREV(0), key);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id;  // where inserted value is
}

int ListInsertAfterId (List * list, int id, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);
    VERIFY_ID(list, id, debug_info);

    // if no room left - realloc
    if (list->fre == list->size) ReallocList (list, list->size + 1);

    int new_id = list->fre;
    list->fre = NEXT(new_id);

    list_elem_t l_elem = {key, DEFAULT_LIST_ELEM_VALUE};
    AssignListEl (&DATA(new_id), &l_elem);

    int old_nxt = NEXT(id);

    NEXT(id) = new_id;
    PREV(new_id) = id;

    NEXT(new_id) = old_nxt;

    PREV(old_nxt) = new_id;
    list->cells_used++;
    list->elems_stored++;

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id; // where inserted value is
}

int ListInsertBefore (List * list, int id, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);
    VERIFY_ID(list, id, debug_info);

    int new_id = InsertAfterIdList(list, PREV(id), key);

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return new_id;
}

int ListInsertSorted (List * list, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST (list, debug_info);

    int    new_id  = -1;
    size_t curr_id = NEXT (0);

    if (list->size == 1)
    {
        new_id = InsertBeginList (list, key);

        goto verify_and_return;
    }

    while (strcmp (DATA (curr_id).key, key) < 0)
    {
        if (curr_id == PREV (0))
        {
            new_id = InsertEndList (list, key);

            goto verify_and_return;
        }

        curr_id = NEXT (curr_id);
    }

    if (strcmp (DATA (curr_id).key, key) == 0)
    {
        DATA (curr_id).value++;
        new_id = curr_id;

        goto verify_and_return;
    }

    new_id = InsertAfterIdList (list, curr_id, key);

verify_and_return:

    ON_DEBUG (VERIFY_LIST (list, debug_info));

    return new_id;
}

list_elem_t ListIdDelete (List * list, int id, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);
    VERIFY_ID(list, id, debug_info);

    int prev = PREV(id);
    int next = NEXT(id);

    NEXT(prev) = next;
    PREV(next) = prev;

    list_elem_t deleted_el = DATA(id);
    AssignListEl (&(DATA(id)), &LIST_POISON);

    PREV(id) = -1;
    NEXT(id) = list->fre;
    list->fre = id;
    list->cells_used--;

    ON_DEBUG(VERIFY_LIST(list, debug_info));

    return deleted_el;
}

int ListKeyDelete (List * list, ht_key_t key, ListDebugInfo debug_info)
{
    VERIFY_LIST(list, debug_info);

    int id = GetIdListKey (list, key);

    if (id != -1)
        DeleteIdList(list, id);

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
    for (int i = list->fre, cnt = 0; i != list->size && i != -1; i = NEXT(i), cnt++)
    {
        if (cnt > list->size || i > list->size)
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

int IsListElemEq (const list_elem_t el_1, const list_elem_t el_2)
{
    return strcmp (el_1.key, el_2.key);
}

int AssignListEl (list_elem_t *el_dst, const list_elem_t *el_src)
{
    if (!el_dst || !el_src) return 1;

    memcpy (el_dst, el_src, sizeof (list_elem_t));

    return 0;
}

int IncreaseValListId (List *list, int id)
{
    assert (list);

    if (id < 0) return 1;

    DATA(id).value++;
    list->elems_stored++;

    return 0;
}
