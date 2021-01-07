// Sample for DynamicArrays working with primitive variables

#include <stdio.h>              /* printf */
#include "DynHeaders/dynarr.h"  /* var types, Dynarr types and Dynarr functions */

// Function prototyping
int IsEven (const void*);
int IsOdd (const void*);
int IsMultipleOfThree (const void*);
int SortByPair (const void*, const void*);

int main()
{
    // Declare variables
    int counter = 0;

    printf(
        "This demo demonstrates usage of some functions related to DynamicArrays.\n"
        "Each DynamicArray \"header\" occupies %d bytes in the memory stream.\n"
        "Let's start by creating a DynamicArray of type Int with enough storage for 5 elements.\n\n"
        , (int)sizeof(DynamicArray)
    );

    // Instantiate a dynamic array with room for 5 elements
    struct DynamicArray original = NewDynamicArray(5, Int);
    DynarrPrintAll(&original, ", ");

    printf("As you can see, it's empty. Let's add some crap to it.\n");
    printf("Like 25 different ints.\n\n");

    // Add a bunch of crap to it
    for (counter = 25; counter >= 0; counter--)
        DynarrAdd(&original, &counter);

    // Print just the array
    printf("\n1- Original DynamicArray - Printing only the array\n");
    DynarrPrint(&original, ", ");

    // Sort it and print the array and its stats too
    printf("\n\n\n2- Original DynamicArray - Sorted in ascending order, with full print\n");
    DynarrSort(&original);
    DynarrPrintAll(&original, ", ");

    // Instantiate a dynamic array that contains a sub-array of "original" from indices 3 to 17
    struct DynamicArray range = DynarrGetSubarray(&original, 3, 17);
    printf("\n3- Range DynamicArray - A sub-array of \"Original\" from indices 3 to 17\n");
    DynarrPrintAll(&range, ", ");


    // Let's remove all even numbers from it
    printf("\n4- Range DynamicArray - After removing all odd numbers\n");
    DynarrRemoveCustom(&range, IsOdd);
    DynarrPrintAll(&range, ", ");

    // Let's instantiate more sub-arrays, this time with custom rules
    // This one contains only even numbers
    printf("\n5- Evens DynamicArray - Contains all even numbers from \"Original\"\n");
    struct DynamicArray evens = DynarrGetSubarrayCustom(&original, IsEven);
    DynarrPrintAll(&evens, ", ");

    // Now let's remove some of it, whatever is at index 2
    printf("\n6- Evens DynamicArray - With 6 elements less, all removed from index 2\n");
    for (counter = 0; counter < 6; counter++)
        DynarrRemove(&evens, 2);

    DynarrPrintAll(&evens, ", ");

    // Time to save some memory by deallocating what we are not using
    printf("\n7- Evens DynamicArray - Now with efficient memory usage\n");
    DynarrTrim(&evens);
    DynarrPrintAll(&evens, ", ");

    // Let's instantiate this one to multiples of 3 only
    printf("\n8- Multi3 DynamicArray - Contains all multiples of 3 from \"Original\"\n");
    struct DynamicArray multi3 = DynarrGetSubarrayCustom(&original, IsMultipleOfThree);
    DynarrPrintAll(&multi3, ", ");

    // Let's join Multi3 with Range, which contains all even numbers from the Original array
    // And after that, let's insert another array in the middle of Multi3
    // Let's try to keep memory allocation as efficient as possible. Start by allocating all
    // the memory you need.
    DynarrExpand(&multi3, range.Count + evens.Count);
    printf("\n9- Multi3 DynamicArray - Should have a lot of extra room now!\n");
    DynarrPrintAll(&multi3, ", ");

    // Now let's join it with Range
    printf("\n10- Multi3 DynamicArray - Joined with Range\n");
    DynarrJoin(&range, &multi3);
    DynarrPrintAll(&multi3, ", ");

    // We can also insert one array into another
    printf("\n11- Multi3 DynamicArray - Inserting \"Evens\" into \"Multi3\" at index 3\n");
    DynarrInsert(&evens, &multi3, 3);
    DynarrPrintAll(&multi3, ", ");

    // It looks very messy, so let's sort it by descending order
    printf("\n12- Multi3 DynamicArray - Sorted by descending order\n");
    DynarrSortDesc(&multi3);
    DynarrPrint(&multi3, ", ");

    // Let's do a customized sort!
    printf("\n\nNow, remember the \"Original\" array?\n");
    DynarrPrint(&original, ", ");
    printf("\nLet's sort it by evens and odds!\n\n");

    printf("13- Original DynamicArray - Sorted by evens and odds in ascending order\n");
    DynarrSortCustom(&original, SortByPair);
    DynarrPrintAll(&original, ", ");

    // Deallocate the whole thing
    printf("\nIt's time to say goodbye. We are deallocating everyone.\n");
    DynarrClear(&original);
    DynarrClear(&range);
    DynarrClear(&evens);
    DynarrClear(&multi3);

    // Try to print one of them after deallocation
    printf("This is what happens when you try to print a deallocated DynamicArray.\n");
    printf("\n14- Original DynamicArray - After memory deallocation\n");
    DynarrPrintAll(&original, ", ");

    printf("\nThat's it!\nThanks for your time.\n");

    return 0;
}

// Checks if a number of any integer type is even.
// Returns non-zero if even, zero if odd.
int IsEven (const void* number)
{
    return !(*(int*)number & 1);
}

// Checks if a number of any integer type is odd.
// Returns non-zero if odd, zero if even.
int IsOdd (const void* number)
{
    return *(int*)number & 1;
}

// Checks if a number of any integer type is multiple of 3.
// Returns non-zero if it is, zero if not.
int IsMultipleOfThree (const void* number)
{
    return !(*(int*)number % 3);
}

// Sorts an array so even numbers come first, then
// odd numbers, in ascending order. I wrote this in
// the laziest way possible, so it's shit.
int SortByPair (const void* num1, const void* num2)
{
    if (*(short*)num1 % 2 == 0 && *(short*)num2 % 2 == 0)
    {
        if (*(short*)num1 < *(short*)num2)
            return -1;
        else
            return 1;
    }
    else if (*(short*)num1 % 2 == 0 && *(short*)num2 % 2 != 0)
    {
        return -1;
    }
    else if (*(short*)num1 % 2 != 0 && *(short*)num2 % 2 == 0)
    {
        return 1;
    }
    else
    {
        if (*(short*)num1 < *(short*)num2)
            return -1;
        else
            return 1;
    }
}
