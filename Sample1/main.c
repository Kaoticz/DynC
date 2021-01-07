// Sample for DynamicVariables (var type)

#include <stdio.h>              /* printf */
#include "dynvar.h"  /* var type and all Dynvar functions */

struct SomeObj
{
    int x;
    int y;
};

int main()
{
    var test = NewDynvar();         // Initialize a "var" variable
    test = DynvarNewInt(&test, 2);  // Assign it an int of value 2
    DynvarPrint(&test, "\n");

    test = DynvarNewDouble(&test, 7.534);   // Assign it a double of value 7.543
    DynvarPrint(&test, "\n");

    test = DynvarNewString(&test, "derp blep"); // Assign it a string "derp blep"
    DynvarPrint(&test, "\n");

    var test2 = DynvarCopy(&test);  // Copy test into test2
    DynvarPrint(&test2, "\n\n");

    struct SomeObj object = {3, 9};     // Create some whatever struct with 2 ints in it
    test = DynvarNewCustom(&test, &object, sizeof(object)); // Assign it to the "var" variable
    printf("x = %d\ny = %d\n\n", *(int*)test.Data, *(int*)(test.Data + 4));
    DynvarPrint(&test2, "\n");

    DynvarClear(&test);
    DynvarClear(&test2);

    return 0;
}