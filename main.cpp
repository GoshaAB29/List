#include "list.h"

void test ();

int main ()
{
    test ();

    return 0;
}

void test ()
{
    printf ("List test: start\n");

    List lst = {};

    List_Ctor (&lst, 120);
    List_Insert_After (&lst, 0, 11);
    List_Insert_After (&lst, 0, 22);
    List_Insert_After (&lst, 2, 33);
    List_Insert_After (&lst, 0, 44);
    List_Insert_After (&lst, 0, 55);
    List_Insert_After (&lst, 0, 66);
    List_Insert_After (&lst, 0, 77);
    List_Insert_After (&lst, 0, 88);
    List_Linearize (&lst);

    List_Delete (&lst, 5);

    for (int i = 1; i < 3; i++) {
        List_Insert_After (&lst, i, 5*i);
    }
    List_Insert_After (&lst, 0, 199);

    List_Insert_After (&lst, 1, 20);
    List_Insert_After (&lst, 1, 30);

    List_Dump_Text (&lst);

    List_Dump (&lst);

    List_Dtor (&lst);

    List_Ctor (&lst, 30);
    List_Ctor (&lst, 30);

    List_Dtor (&lst);
    List_Dtor (&lst);

    printf ("List test: finish\n");
}
