// Sample for DynamicArrays working with DynamicVariables

#include <stdio.h>              /* printf */
#include "dynvar.h"             /* var types and Dynvar functions */
#include "dynarr.h"             /* Dynarr types and Dynarr functions */

int main()
{
    struct DynamicArray secularArray = NewDynamicArray(5, Var);
    var test = NewDynvar();

    test = DynvarNewUShort(&test, 21);
    DynarrAdd(&secularArray, &test);

    test = DynvarNewUChar(&test, 'b');
    DynarrAdd(&secularArray, &test);

    test = DynvarNewFloat(&test, 12.3456);
    DynarrAdd(&secularArray, &test);

    test = DynvarNewLLong(&test, 9876543210);
    DynarrAdd(&secularArray, &test);

    test = DynvarNewString(&test, "bippidy boppidy");
    DynarrAdd(&secularArray, &test);

    test = DynvarNewInt(&test, -13);
    DynarrAdd(&secularArray, &test);

    test = DynvarNewUShort(&test, 5);
    DynarrAdd(&secularArray, &test);

    DynarrPrintAll(&secularArray, ", ");

    // The elements in the array will be sorted
    // According to their value and type. From smallest
    // to biggest: signed types, unsigned types,
    // floating-point types, strings and DynVars
    DynarrSort(&secularArray);
    DynarrPrintAll(&secularArray, ", ");

    test = DynvarNewString(&test, "bippidy boppidy");
    printf("\nIndexOf 5 = [%d]\n", DynarrIndexOf(&secularArray, &test));

    DynarrClear(&secularArray);
    DynvarClear(&test);

    return 0;
}