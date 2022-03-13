#include "list.h"
#include "./graphviz_dump/graph_dump.h"
#include <assert.h>

//#define DUMP_AUTOSTART

void List_Ctor (List *lst, unsigned int capacity)
{
    if ((lst == nullptr) || (capacity <= 0))
        return;

    lst->data = (elem_t*)calloc (capacity + 1, sizeof (elem_t));
    lst->prev = (int*)calloc (capacity + 1, sizeof (int));
    lst->next = (int*)calloc (capacity + 1, sizeof (int));

    lst->free = 1;
    lst->head = 1;
    lst->tail = 1;

    lst->capacity = capacity;
    lst->size = 0;

    lst->linearize = 1;
    lst->fictitious = 0;

    for (size_t i = 1; i <= capacity; i++) {
        lst->data[i] = ERROR_VALUE;

        lst->prev[i] = -1;
        lst->next[i] = -(i + 1);

    }

    lst->next[lst->capacity] = lst->fictitious;

    lst->prev[lst->fictitious] = lst->fictitious;
    lst->next[lst->fictitious] = lst->fictitious;

}

int List_Insert_After (List *lst, unsigned int num_pos, elem_t value) //добавляет элемент после элемента, добавленного num_pos-ым.
{
    int error = List_Verify (lst);
    if (error != LIST_IS_OK)
        return error;

    if (lst->size == lst->capacity - 1)
        List_Resize (lst, lst->capacity * MEMORY_REALLOCATE_UP);

    unsigned int free = lst->free;
    lst->free = abs(lst->next[free]);

    if (free != lst->prev[lst->fictitious] + 1)
        lst->linearize = 0;

    lst->data[free] = value;

    lst->next[free] = lst->next[num_pos];
    lst->next[num_pos] = free;
    lst->prev[free] = num_pos;
    lst->prev[lst->next[free]] = free;

    lst->size++;
    lst->head = lst->next[lst->fictitious];
    lst->tail = lst->prev[lst->fictitious];
    return 0;
}

int List_Delete (List* lst, unsigned int num_pos_del)
{
    int error = List_Verify (lst);
    if (error != LIST_IS_OK)
        return error;

    if ((lst->size < lst->capacity / MEMORY_REALLOCATE_DOWN - 1) &&
        (lst->capacity > MIN_CAPACITY))
            List_Resize (lst, lst->capacity / MEMORY_REALLOCATE_DOWN);

    lst->next[lst->prev[num_pos_del]] = lst->next[num_pos_del];
    lst->prev[lst->next[num_pos_del]] = lst->prev[num_pos_del];

    lst->next[num_pos_del] = -(lst->free);
    lst->prev[num_pos_del] = -1;
    lst->data[num_pos_del] = ERROR_VALUE;
    lst->free = num_pos_del;

    lst->size--;
    lst->head = lst->next[lst->fictitious];
    lst->tail = lst->prev[lst->fictitious];
    return 0;
}

void List_Dtor (List* lst)
{
    int error = List_Verify (lst);
    if ((error == LIST_IS_DESTRUCTED) || (error == LIST_PTR_NULL))
        return;

    lst->head = ERROR_VALUE;
    lst->tail = ERROR_VALUE;

    for (int i = 0; i <= lst->capacity; i++) {
        lst->data[i] = ERROR_VALUE;
        lst->prev[i] = ERROR_VALUE;
        lst->next[i] = ERROR_VALUE;
    }

    lst->size = ERROR_VALUE;
    lst->capacity = ERROR_VALUE;
    lst->free = ERROR_VALUE;
    lst->linearize = ERROR_VALUE;
    lst->fictitious = ERROR_VALUE;

    free (lst->data);
    free (lst->next);
    free (lst->prev);

    lst->data = (elem_t*)ERROR_PTR;
    lst->prev = (int*)ERROR_PTR;
    lst->next = (int*)ERROR_PTR;
}

int List_Verify (List* lst)
{
    if (lst == nullptr)
        return LIST_PTR_NULL;

    if ((lst->data == (elem_t*)ERROR_PTR) && (lst->prev == (int*)ERROR_PTR) &&
        (lst->next == (int*)ERROR_PTR))
        return LIST_IS_DESTRUCTED;

    if ((lst->data == nullptr) || (lst->capacity <= 0) || (lst->size < 0) ||
        (lst->size > lst->capacity) || (abs(lst->linearize) > 2))
        return LIST_IS_BAD;

    return LIST_IS_OK;
}

void List_Dump_Text (List* lst)
{
    FILE* log = fopen ("log_list.txt", "w");

    fprintf (log, "Dump of [%p] list:\n", lst);
    fprintf (log, "free is %d\n", lst->free);
    fprintf (log, "head is [%d], tail is [%d]\n", lst->head, lst->tail);
    for (int i = 0; i <= lst->capacity; i++) {
        fprintf (log, "number is %d, data is [%d], next is [%d], prev is [%d]\n", i,
                 lst->data[i], lst->next[i], lst->prev[i]);
    }
}

void List_Dump (List* lst)
{
    FILE* visdump = fopen ("dump.dot", "w");

    Dotdump_Start (visdump, lst);

    for (int num_elem = 0; num_elem <= lst->size; num_elem++)
        Dotdump_Create_Node (visdump, lst, num_elem);

    Dotdump_Node_Connection (visdump, lst);

    Dotdump_End (visdump);

    fclose (visdump);

    system ("dot dump.dot -T png -o dump.png");
    system ("rm dump.dot");

    #ifdef DUMP_AUTOSTART
    system ("start dump.png");
    #endif DUMP_AUTOSTART
}

int List_Resize (List* lst, int upd_capacity)
{
    if (List_Verify (lst) != LIST_IS_OK)
        return 0;

    lst->data = (elem_t*)realloc (lst->data, (upd_capacity + 1) * sizeof(elem_t));
    lst->next = (int*)realloc (lst->next, (upd_capacity + 1) * sizeof(int));
    lst->prev = (int*)realloc (lst->prev, (upd_capacity + 1) * sizeof(int));

    for (int i = lst->capacity; i < upd_capacity; i++)
    {
        lst->next[i] = -lst->free;
        lst->prev[i] = -1;
        lst->data[i] = ERROR_VALUE;

        lst->free = i;

    }

    lst->next[upd_capacity] = lst->fictitious;

    lst->capacity = upd_capacity;
    return 0;
}

List* List_Linearize (List* lst)
{
    if (List_Verify (lst) != LIST_IS_OK)
        return 0;

    List* linear_lst = (List*)calloc (1, sizeof (List));
    List_Ctor (linear_lst, lst->capacity);

    int physical_num = 0;

    linear_lst->data[lst->fictitious] = 0;
    linear_lst->next[lst->fictitious] = lst->fictitious + 1;
    linear_lst->prev[lst->fictitious] = lst->size;

    for (int current_pos = linear_lst->fictitious + 1; current_pos <= lst->size; current_pos++) {
        physical_num = Very_Very_Slow_Logical_To_Physical (lst, current_pos);
        linear_lst->data[current_pos] = lst->data[physical_num];
        linear_lst->next[current_pos] = current_pos + 1;
        linear_lst->prev[current_pos] = current_pos - 1;
    }

    linear_lst->next[lst->size] = lst->fictitious;

    memcpy (lst->data, linear_lst->data, (linear_lst->capacity + 1) * sizeof(elem_t));
    memcpy (lst->next, linear_lst->next, (linear_lst->capacity + 1) * sizeof(int));
    memcpy (lst->prev, linear_lst->prev, (linear_lst->capacity + 1) * sizeof(int));

    lst->linearize = 1;
    lst->free = lst->size + 1;
    lst->head = lst->next[lst->fictitious];
    lst->tail = lst->prev[lst->fictitious];

    List_Dtor (linear_lst);

    return lst;
}

int Very_Very_Slow_Logical_To_Physical (List* lst, int logic_num)
{
    int current = lst->fictitious;

    for (int i = 0; i < logic_num; i++)
        current = lst->next[current];

    return current;
}
