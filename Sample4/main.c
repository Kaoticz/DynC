#include <stdio.h>                  /* printf */
#include "dynlst.h"                 /* All Dynlst functions */

int main()
{
    Dynlst list = NewDynlst();      // Create a new linked list
    var x =  NewDynvar();           // Create a dynamic variable

    x = DynvarNewInt(&x, 99);       // Assign the dynvar some value
    DynlstPush(&list, &x, Var);     // Add the dynvar to the linked list

    // Add a bunch of stuff
    // 3 strings
    DynlstPush(&list, "testing", String);
    DynlstPush(&list, "crap", String);
    DynlstPush(&list, "at midnight", String);

    // Another dynvar
    x = DynvarNewFloat(&x, 1.23456);
    DynlstPush(&list, &x, Var);

    // A string, a long double and a uint
    DynlstPush(&list, "derp", String);

    long double y = 9.876543210;
    DynlstPush(&list, &y, LDouble);

    unsigned int z = 4545;
    DynlstPush(&list, &z, UInt);

    // Another dynvar
    x = DynvarNewString(&x, "I'm a var!");
    DynlstPush(&list, &x, Var);

    printf("Printing the entire list:\n");
    DynlstPrintAll(&list, ", ");

    printf("Printing a specific element:\nlist[4] = ");
    DynlstPrintElem(&list, 4, "\n\n");

    // Add 2 strings to the beginning of the linked list
    DynlstEnqueue(&list, "123", String);
    DynlstEnqueue(&list, "456", String);

    printf("Printing the entire list after enqueueing string \"123\" and \"456\" :\n");
    DynlstPrintAll(&list, ", ");

    // Changing an element
    printf("Changing the element at index [1]: it was \"");
    DynlstPrintElem(&list, 1, "\", ");
    printf("now it is \"");
    DynlstSet(&list, "ooga booga", 1, String);
    DynlstPrintElem(&list, 1, "\".\n");

    // Print the entire list again
    DynlstPrintAll(&list, ", ");

    // Find the index of an element by the data it stores. Returns -1 if it is not found
    printf("Find index of:\n");
    printf("\"crap\": [%d]\n", DynlstIndexOf(&list, "crap", 5));
    printf("\"whoops\": [%d]\n\n", DynlstIndexOf(&list, "whoops", 7));

    // Pop removes an element at the end of the list
    // Dequeue does the same
    printf("Popping removes an element at the end of the list:\n");
    DynlstPop(&list); // Same as "DynlstDequeue(&list);"
    DynlstPrint(&list, ", ");

    // Use Remove to remove from the beginning of the list or any other location
    printf("\n\nRemove removes any element in the list: [0]\n");
    DynlstRemove(&list, 0);
    DynlstPrint(&list, ", ");

    // Use Add to add an element anywhere in the list
    printf("\n\nAdd adds an element anywhere in the list: [6]\n");
    DynlstAdd(&list, "AAAAAAA", 6, String);
    DynlstPrint(&list, ", ");

    printf("\n\nFinal result:\n");
    DynlstPrintAll(&list, ", ");

    // Nuke everything
    DynlstClear(&list);
    DynvarClear(&x);

    printf("Nuked linked list:\n");
    DynlstPrintAll(&list, ", ");

    return 0;
}