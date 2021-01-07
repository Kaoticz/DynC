 /**
 * \file        dynarr.h
 * \author      Kotz#7922
 * \version     1.0
 * \copyright   <a href="https://www.apache.org/licenses/LICENSE-2.0">Apache Software License</a> (ASL)
 * \date        17/09/2020
 * \brief       Definition for a DynamicArray and its supporting functions.
 * \details     A dynamic array is an array that expands and shrinks according to the program's needs.
 * <br> This implementation achieves this by defining a DynamicArray struct that contains a pointer
 * to the array and a few other variables to help keep track of relevant information about the array.
 * Use the functions in this library to manipulate the data contained in the array of this struct.
 * <br><br>
 * The array is referenced by a void pointer, so the user can conveniently store any type of
 * data in it without worrying about its instantiation details. This does not mean that one DynamicArray
 * can store data of multiple types simultaneously. The functions will silently fail if you try to do so.
 * <br><br>
 * To keep CPU load at a minimum, memory is allocated in bulk (rather than every time something is added
 * or removed), always twice as much than its current size (i.e. the array doubles every time its limit
 * is exceeded). This does not happen when removing elements from the array (i.e. if you remove 59
 * elements from an array with 60 elements, its capacity will remain unchanged). Use the functions present
 * in this library to decrease the amount of memory allocated to that array.
 * <br><br>
 * Built with GCC 8.1.0 (x86_64-posix-seh-rev0) on Windows 10.
 */

#ifndef DYNARR_H    // Only include this header file if it hasn't been included in the calling file already
#define DYNARR_H

#include <stdio.h>      /* printf, fprintf */
#include <stdlib.h>     /* qsort, calloc, realloc, free */
#include <string.h>     /* memcpy, memcmp, memmove */
#include "dynvar.h"     /* All Dynvar functions */

#ifndef _DYN_DTYPES
    #define _DYN_DTYPES
    /**
     * This global enum represents a data type. <br>
     * Use code completion to see the available types.
     */
    enum DataType {
        Char = 1, Short, Int, Long, LLong,  // Signed integer types
        UChar, UShort, UInt, ULong, ULLong, // Unsigned integer types
        Float, Double, LDouble,             // Floating-point types
        String, Custom, Var, Ptr            // Strings, structs, DynamicVariables and pointers
    };
#endif // _DYN_DTYPES

/**
 * This object represents an array that's able to safely expand its storage space as needed. <br>
 * Please, do not manipulate its members directly. Use the functions prefixed with <i>Dynarr</i>
 * to handle tasks related to this object. Use code completion to check all available functions. <br>
 * Use the <u>NewDynamicArray()</u> function to instantiate a new DynamicArray. <br>
 * Check the \c enum \c DataType with code completion to see all supported data types.
 */
typedef struct DynamicArray
{
    void* Array;                /**< Pointer that points to the beginning of the array. */
    unsigned short Capacity;    /**< Represents the amount of elements the array is able to store before requiring a memory reallocation. */
    unsigned short Count;       /**< Represents the amount of elements stored in the array. */
    unsigned short Type;        /**< Represents the data type of the elements stored in the array. */
    unsigned short TypeSize;    /**< Represents the size of the data type of the elements stored in the array, in bytes. */
}DynamicArray, Dynarr;          /**< Alias for declaring a DynamicArray. It's equivalent to <u>struct DynamicArray vName</u> or <u>DynamicArray vName</u>. */


// Function prototyping
extern struct DynamicArray NewDynamicArray (int, enum DataType);
extern unsigned short DynarrAdd (struct DynamicArray*, void*);
extern void DynarrJoin (struct DynamicArray*, struct DynamicArray*);
extern int DynarrInsert (struct DynamicArray*, struct DynamicArray*, int);
extern int DynarrInsertElem (void*, struct DynamicArray*, int, enum DataType);
extern int DynarrInsertArray (void*, struct DynamicArray*, int, int, enum DataType);
int DynarrIndexOf (struct DynamicArray*, void*);
extern int DynarrRemove (struct DynamicArray*, int);
extern int DynarrRemoveCustom (struct DynamicArray*, int (*)(const void*));
extern void* DynarrGetElement(struct DynamicArray*, int);
extern void DynarrSetElement(struct DynamicArray*, void*, int);
extern void DynarrPrint (struct DynamicArray*, char*);
extern void DynarrPrintAll (struct DynamicArray*, char*);
extern int DynarrTrim (struct DynamicArray*);
extern void DynarrExpand (struct DynamicArray*, int);
extern void DynarrClear (struct DynamicArray*);
extern void DynarrSort (struct DynamicArray*);
extern void DynarrSortDesc (struct DynamicArray*);
extern void DynarrSortCustom (struct DynamicArray*, int (*)(const void*, const void*));
extern struct DynamicArray DynarrGetSubarray (struct DynamicArray*, int, int);
extern struct DynamicArray DynarrGetSubarrayCustom (struct DynamicArray*, int (*)(const void*));

#endif // _DYN_ARR
