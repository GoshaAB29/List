#include "graph_dump.h"
#include "../list.h"

void Dotdump_Start (FILE* visdump, List* lst)
{
    fputs ("digraph List_Dump {\n", visdump);
    fputs ("rankdir = TB\n", visdump);

    fputs ("node [color = red, shape = box, style =\"rounded, filled\","
           "fontsize = 15]\n", visdump);

    fprintf (visdump, "Data [shape=doublecircle color = green slades = 1.4 distortion = -.8,"
             "label = \"List parameters:\n\n"
             "size = %ld; capacity = %d\n \n"
             "head = %d; tail= %d; free = %d\n\n"
             "linearize = %d\\n\"]\n", lst->size, lst->capacity,
             lst->head, lst->tail, lst->free, lst->linearize);
}

void Dotdump_Create_Node (FILE* visdump, struct List* lst, int num)
{
    fprintf (visdump, "Node%d [shape=record,"
             "label=\"%d|data\\n %d|next\\n %d|prev\\n %d\"]\n", num,
             lst->prev[lst->next[num]], lst->data[num],lst->next[num], lst->prev[num]);
}

void Dotdump_Node_Connection (FILE* visdump, struct List* lst)
{
    int iter = lst->next[lst->fictitious];

    while (iter != lst->prev[lst->fictitious]) {
        fprintf (visdump, "Node%d ->", iter);
        iter = lst->next[iter];
    }
    fprintf (visdump, "Node%d\n\n", lst->prev[lst->fictitious]);

    fprintf (visdump, "Node%d -> Node%d\n", lst->fictitious,
             lst->next[lst->fictitious]);
    fprintf (visdump, "Node%d -> Node%d\n", lst->prev[lst->fictitious],
             lst->fictitious);
}

void Dotdump_End (FILE* visdump)
{
    fputs ("}\n", visdump);
}
