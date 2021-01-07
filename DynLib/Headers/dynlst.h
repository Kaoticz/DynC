/**
* \file        dynlst.h
* \author      Kotz#7922
* \version     1.0
* \copyright   <a href="https://www.apache.org/licenses/LICENSE-2.0">Apache Software License</a> (ASL)
* \date        27/09/2020
* \brief       Definition for a DynamicList and its supporting functions.
* \details     A dynamic list is a linked list that expands and shrinks according to the program's needs.
* <br> This implementation achieves this by defining a DynlstHeader struct that works as a header for
* the list, keeping track of the first and last elements in the list, as well as some basic information about
* the data stored in it. Its elements are comprised of a __DynlstElem struct, which effectively encapsulates
* any data of interest. Direct manipulation, however, is discouraged. Use the functions defined in this library
* instead.
* <br><br>
* The linked list can behave either as a stack (if you only use the <u>DynlstPush()</u> and <u>DynlstPop()</u>
* functions) or as a queue (if you only use the <u>DynlstEnqueue()</u> and <u>DynlstDequeue()</u> functions).
* Additionally, you are also free to add and remove new elements anywhere in the list.
* <br><br>
* Linked lists use considerably more memory and are slower to process compared to dynamic arrays. On the other
* hand, they don't need to be stored contiguously in the memory and are often used as a base for other,
* more complex, data structures.
* <br><br>
* Built with GCC 8.1.0 (x86_64-posix-seh-rev0) on Windows 10.
*/

#ifndef _DYNLST_H       // Only include this header file if it hasn't been included in the calling file already
#define _DYNLST_H

#include <stdio.h>      /* printf, fprintf */
#include <stdlib.h>     /* malloc, free */
#include <string.h>     /* memcmp */
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

#ifndef _DYN_LST_MACROS
#define _DYN_LST_MACROS

#define NewDynlist() NewDynlst()        /**< Equivalent to <u>NewDynlst()</u> */
/**
 * Convenience macro to instantiate a DynamicList for the first time.
 * Using dynamic lists without properly instantiating them may cause <i>undefined behavior</i>.
 */
#define NewDynlst() {               \
            .First = NULL,              \
            .Last = NULL,               \
            .Count = 0,                 \
            .Size = 0,                  \
            .TotalSize = sizeof(Dynlst) \
        }

#endif // _DYN_LST_MACROS

/**
 * This object represents the element of a list. It contains a pointer to the stored data, as well as pointers
 * to the next and previous list elements. The \c Prev pointer of the first element and the \c Next pointer of
 * the last element will always be NULL. It also keeps track of basic information about the stored data. <br>
 * Please, avoid manipulating its members directly or indirectly.
 */
typedef struct __DynlstElem
{
    void* Data;                 /**< Pointer to the stored data. */
    struct __DynlstElem* Next;  /**< Pointer to the next element in the list */
    struct __DynlstElem* Prev;  /**< Pointer to the previous element in the list */
    unsigned short Type;        /**< Represents the data type of the data stored in this list element. */
    unsigned short TypeSize;    /**< Represents the size of the data type of the data stored in the list element, in bytes. */
    unsigned int DataSize;      /**< Represents the storage space occupied by the stored data in the memory stream, in bytes */
}__DynlstElem;

/**
 * This object represents the header of a linked list. It contains a pointer to the first and last elements of
 * the list, as well as some basic information about the list. <br>
 * Please, do not manipulate its members directly. Use the functions prefixed with <i>Dynlst</i> to handle tasks
 * related to this object. Use code completion to check all available functions. <br>
 * Use the <u>NewDynlst()</u> macro to instantiate a new DynamicList.
 */
typedef struct DynlstHeader
{
    struct __DynlstElem* First; /**< Pointer to the first element in the linked list. */
    struct __DynlstElem* Last;  /**< Pointer to the last element in the linked list. */
    unsigned int Count;         /**< Amount of elements contained in the list. */
    unsigned int Size;          /**< Storage space allocated for all the stored data in the list, in bytes. */
    unsigned int TotalSize;     /**< Storage space allocated for all the stored data in the list, for the element objects themselves and the list header. */
}DynamicList, Dynlst;           /**< Alias for declaring a DynamicList. It's equivalent to <u>struct DynlstHeader lName</u> or <u>Dynlst lName</u>. */

// Function prototyping
extern void DynlstAdd (struct DynlstHeader*, void*, int, enum DataType);
extern void DynlstPush (struct DynlstHeader*, void*, enum DataType);
extern void DynlstPop (struct DynlstHeader*);
extern void DynlstEnqueue (struct DynlstHeader*, void*, enum DataType);
extern void DynlstDequeue (struct DynlstHeader*);
extern void DynlstRemove (struct DynlstHeader*, int);
extern void DynlstClear (struct DynlstHeader*);
extern void DynlstPrint (struct DynlstHeader*, char*);
extern void DynlstPrintAll (struct DynlstHeader*, char*);
extern void DynlstPrintElem (struct DynlstHeader*, int, char*);
extern void DynlstSet (struct DynlstHeader*, void*, int, enum DataType);
extern int DynlstIndexOf (struct DynlstHeader*, void*, int);
extern struct __DynlstElem* DynlstGet (struct DynlstHeader*, int);

#endif // _DYNLST_H