#ifndef LIST_INCL
#define LIST_INCL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef int elem_t;

#define LIST_PTR_NULL (unsigned int)(-10)
#define LIST_IS_BAD (unsigned int)(-11)
#define LIST_IS_OK (unsigned int)(-12)
#define LIST_IS_DESTRUCTED (unsigned int)(-13)

#define LIST_FMT "%d"

const static int MEMORY_REALLOCATE_DOWN = 8;
const static int MEMORY_REALLOCATE_UP = 3;
const static int MIN_CAPACITY = 4;

const static elem_t ERROR_VALUE = (elem_t)(-1);
const static elem_t* ERROR_PTR = (elem_t*)103;

struct List {
    elem_t* data = (elem_t*)ERROR_PTR;
    int* prev = (int*)ERROR_PTR;
    int* next = (int*)ERROR_PTR;

    int linearize = 0;
    int fictitious = 0;

    unsigned int head = 0;
    unsigned int tail = 0;

    size_t size = 0;
    unsigned int capacity = 1;
    unsigned int free = 0;
    };

    void List_Ctor (List* lst, unsigned int capacity);
    void List_Dtor (List* lst);
    int List_Verify (List* lst);

    void List_Dump_Text (List* lst);
    void List_Dump (List* lst);

    int List_Insert_After (List* lst, unsigned int num_pos, elem_t value);
    int List_Delete (List* lst, unsigned int num_pos_del);

    int List_Resize (List* lst, int upd_capacity);
    List* List_Linearize (List* lst);

    int Very_Very_Slow_Logical_To_Physical (List* lst, int logic_num);

#endif
