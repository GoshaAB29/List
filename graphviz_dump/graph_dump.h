#ifndef GRAPH_DUMP
#define GRAPH_DUMP

#include "../list.h"
#include <stdio.h>

void Dotdump_Start (FILE* visdump, List* lst);

void Dotdump_Create_Node (FILE* visdump, List* lst, int num_elem);

void Dotdump_Node_Connection (FILE* visdump, List* lst);

void Dotdump_End (FILE* visdump);

#endif GRAPH_DUMP
