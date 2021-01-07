 /**
 * \file        dynvar.h
 * \author      Kotz#7922
 * \version     1.0
 * \copyright   <a href="https://www.apache.org/licenses/LICENSE-2.0">Apache Software License</a> (ASL)
 * \date        15/09/2020
 * \brief       Definition for a DynamicVariable and its supporting functions.
 * \details     A dynamic variable is able to store or reference data of different types.
 * <br> This implementation achieves this by defining a DynamicVariable struct that contains a pointer
 * to the data it's supposed to store. Use the functions in this library to manipulate the data contained
 * stored in these objects.
 * <br><br>
 * The data is referenced by a void pointer, so the user can conveniently store any type of data in it
 * without worrying about its instantiation details. When you assign a dynamic variable to another data,
 * the old data that was previously stored in it will be deallocated and lost forever.
 * <br><br>
 * Built with GCC 8.1.0 (x86_64-posix-seh-rev0) on Windows 10.
 */

#ifndef DYNVAR_H    // Only include this header file if it hasn't been included in the calling file already
#define DYNVAR_H

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* malloc, free */
#include <string.h>     /* strcpy, memcpy */

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

#ifndef _DYN_VAR_MACROS
    #define _DYN_VAR_MACROS
    /**
     * Convenience macro to instantiate a dynamic variable for the first time.
     * Using dynamic variables without properly instantiating them may cause <i>undefined behavior</i>.
     */
    #define NewDynvar() {.Data = NULL, .Type = 0, .Size = 0}
    #define DynVar Var  /**< Alias for a Var enum. */
#endif // _DYN_VAR_MACROS

/**
 * This object represents a dynamic variable. Use the functions prefixed with <i>Dynvar</i>
 * to handle common tasks related to this object. Use code completion to check all available functions. <br>
 * Use the <u>NewDynvar()</u> macro to instantiate a new dynamic variable. <br>
 * Check the \c enum \c DataType with code completion to see all supported data types.
 */
typedef struct DynamicVariable
{
    void* Data;             /**< Pointer that points to the beginning of the stored data. Direct manipulation is not recommended. */
    unsigned short Type;    /**< Represents the data type of the elements stored in the array. */
    unsigned short Size;    /**< Represents the size of the data type of the elements stored in the array, in bytes. */
}Dynvar, var;               /**< Alias for declaring a DynamicVariable. It's equivalent to \c <u>struct DynamicVariable vName</u> or <u>DynamicVariable vName</u>. */


// Function prototyping
extern struct DynamicVariable DynvarCopy (struct DynamicVariable*);
extern void DynvarClear (struct DynamicVariable*);
extern void DynvarPrint (struct DynamicVariable*, char*);
extern struct DynamicVariable DynvarNew (struct DynamicVariable*, void*, enum DataType);
extern struct DynamicVariable DynvarNewCustom (struct DynamicVariable*, void*, int);
extern struct DynamicVariable DynvarNewChar (struct DynamicVariable*, char);
extern struct DynamicVariable DynvarNewShort (struct DynamicVariable*, short);
extern struct DynamicVariable DynvarNewInt (struct DynamicVariable*, int);
extern struct DynamicVariable DynvarNewLong (struct DynamicVariable*, long);
extern struct DynamicVariable DynvarNewLLong (struct DynamicVariable*, long long);
extern struct DynamicVariable DynvarNewUChar (struct DynamicVariable*, unsigned char);
extern struct DynamicVariable DynvarNewUShort (struct DynamicVariable*, unsigned short);
extern struct DynamicVariable DynvarNewUInt (struct DynamicVariable*, unsigned int);
extern struct DynamicVariable DynvarNewULong (struct DynamicVariable*, unsigned long);
extern struct DynamicVariable DynvarNewULLong (struct DynamicVariable*, unsigned long long);
extern struct DynamicVariable DynvarNewFloat (struct DynamicVariable*, float);
extern struct DynamicVariable DynvarNewDouble (struct DynamicVariable*, double);
extern struct DynamicVariable DynvarNewLDouble (struct DynamicVariable*, long double);
extern struct DynamicVariable DynvarNewString (struct DynamicVariable*, char*);

#endif // DYNVAR_H
