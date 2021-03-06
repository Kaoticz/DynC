#include "../DynHeaders/dynvar.h"


/* Private Functions */

static struct DynamicVariable __Dynvar_NewVar (struct DynamicVariable*, void*, enum DataType);


/* Public Functions */


/** \brief Copies one DynamicVariable to another DynamicVariable.
 * \warning This function causes a memory leak if its returned value is not caught.
 * \param[in] source Pointer to the DynamicVariable to be copied from.
 * \retval struct DynamicVariable
 * \arg Returns a copy of the specified DynamicVariable.
 */
struct DynamicVariable DynvarCopy (struct DynamicVariable* source)
{
    struct DynamicVariable copy = *source;
    copy.Data = malloc(copy.Size);
    memcpy(copy.Data, source->Data, copy.Size);

    return copy;
}

/** \brief Deallocates the memory used to store the variable's data and resets all its properties.
 * \warning This function may cause a segmentation fault if the pointer is pointing to data outside of the heap.
 * \warning This function may deallocate memory of an external variable if the pointer is pointing to that variable.
 * \param[out] svar Pointer to the DynamicVariable to be cleared.
 * \return \c void
 */
void DynvarClear (struct DynamicVariable* svar)
{
    free(svar->Data);
    svar->Data = NULL;
    svar->Size = 0;
    svar->Type = 0;
}

/** \brief Assigns the specified data to a DynamicVariable.
 * \remark It does not accept literals. Use the other <u>DynvarNew</u> functions instead.
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data Pointer to the data to be assigned.
 * \param[in] DType The type of data to be assigned.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified value assigned to it.
 */
struct DynamicVariable DynvarNew (struct DynamicVariable* svar, void* data, enum DataType DType)
{
    DynvarClear(svar);
    return __Dynvar_NewVar(svar, data, DType);
}

/** \brief Assigns the specified struct to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] object Pointer to the struct to be assigned.
 * \param[in] dataSize The size of the struct to be assigned.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified struct assigned to it.
 */
struct DynamicVariable DynvarNewCustom (struct DynamicVariable* svar, void* object, int dataSize)
{
    DynvarClear(svar);
    svar->Size = dataSize;
    __Dynvar_NewVar(svar, object, Custom);

    return *svar;
}

/** \brief Assigns the specified \c char to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c char variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c char assigned to it.
 */
struct DynamicVariable DynvarNewChar (struct DynamicVariable* svar, char data)
{
    return DynvarNew(svar, &data, Char);
}

/** \brief Assigns the specified \c short to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c short variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c short assigned to it.
 */
struct DynamicVariable DynvarNewShort (struct DynamicVariable* svar, short data)
{
    return DynvarNew(svar, &data, Short);
}

/** \brief Assigns the specified \c int to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c int variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c int assigned to it.
 */
struct DynamicVariable DynvarNewInt (struct DynamicVariable* svar, int data)
{
    return DynvarNew(svar, &data, Int);
}

/** \brief Assigns the specified \c long to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c long variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c long assigned to it.
 */
struct DynamicVariable DynvarNewLong (struct DynamicVariable* svar, long data)
{
    return DynvarNew(svar, &data, Long);
}

/** \brief Assigns the specified \c long \c long to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c long \c long variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c long \c long assigned to it.
 */
struct DynamicVariable DynvarNewLLong (struct DynamicVariable* svar, long long data)
{
    return DynvarNew(svar, &data, LLong);
}

/** \brief Assigns the specified \c unsigned \c char to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c unsigned \c char variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c unsigned \c char assigned to it.
 */
struct DynamicVariable DynvarNewUChar (struct DynamicVariable* svar, unsigned char data)
{
    return DynvarNew(svar, &data, UChar);
}

/** \brief Assigns the specified \c unsigned \c short to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c unsigned \c short variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c unsigned \c short assigned to it.
 */
struct DynamicVariable DynvarNewUShort (struct DynamicVariable* svar, unsigned short data)
{
    return DynvarNew(svar, &data, UShort);
}

/** \brief Assigns the specified \c unsigned \c int to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c unsigned \c int variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c unsigned \c int assigned to it.
 */
struct DynamicVariable DynvarNewUInt (struct DynamicVariable* svar, unsigned int data)
{
    return DynvarNew(svar, &data, UInt);
}

/** \brief Assigns the specified \c unsigned \c long to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c unsigned \c long variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c unsigned \c long assigned to it.
 */
struct DynamicVariable DynvarNewULong (struct DynamicVariable* svar, unsigned long data)
{
    return DynvarNew(svar, &data, ULong);
}

/** \brief Assigns the specified \c unsigned \c long \c long to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c unsigned \c long \c long variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c unsigned \c long \c long assigned to it.
 */
struct DynamicVariable DynvarNewULLong (struct DynamicVariable* svar, unsigned long long data)
{
    return DynvarNew(svar, &data, ULLong);
}

/** \brief Assigns the specified \c float to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c float variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c float assigned to it.
 */
struct DynamicVariable DynvarNewFloat (struct DynamicVariable* svar, float data)
{
    return DynvarNew(svar, &data, Float);
}

/** \brief Assigns the specified \c double to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c double variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c double assigned to it.
 */
struct DynamicVariable DynvarNewDouble (struct DynamicVariable* svar, double data)
{
    return DynvarNew(svar, &data, Double);
}

/** \brief Assigns the specified \c long \c double to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c long \c double variable.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c long \c double assigned to it.
 */
struct DynamicVariable DynvarNewLDouble (struct DynamicVariable* svar, long double data)
{
    return DynvarNew(svar, &data, LDouble);
}

/** \brief Assigns the specified \c string to a DynamicVariable.
 *
 * \param[in,out] svar Pointer to the DynamicVariable.
 * \param[in] data A \c string.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified \c string assigned to it.
 */
struct DynamicVariable DynvarNewString (struct DynamicVariable* svar, char* data)
{
    return DynvarNew(svar, data, String);
}

/** \brief Prints the data stored in a DynamicVariable.
 *
 * \param[out] svar Pointer to the DynamicVariable.
 * \param[in] separator A string, to be placed after the data. It can be empty.
 * \return \c void
 */
void DynvarPrint (struct DynamicVariable* svar, char* separator)
{
    switch (svar->Type)
    {
        case Char:
            printf("%c%s", *(signed char*)svar->Data, separator);
            break;

        case Short:
            printf("%d%s", *(short*)svar->Data, separator);
            break;

        case Int:
            printf("%d%s", *(int*)svar->Data, separator);
            break;

        case Long:
            printf("%ld%s", *(long*)svar->Data, separator);
            break;

        case LLong:
            #ifdef _WIN32
                __mingw_printf("%lld%s", *(long long*)svar->Data, separator);
            #else
                printf("%lld%s", *(long long*)svar->Data, separator);
            #endif // _WIN32
            break;

        case UChar:
            printf("%c%s", *(unsigned char*)svar->Data, separator);
            break;

        case UShort:
            printf("%u%s", *(unsigned short*)svar->Data, separator);
            break;

        case UInt:
            printf("%u%s", *(unsigned int*)svar->Data, separator);
            break;

        case ULong:
            printf("%lu%s", *(unsigned long*)svar->Data, separator);
            break;

        case ULLong:
            #ifdef _WIN32
                __mingw_printf("%llu%s", *(unsigned long long*)svar->Data, separator);
            #else
                printf("%llu%s", *(unsigned long long*)svar->Data, separator);
            #endif // _WIN32
            break;

        case Float:
            printf("%.6g%s", *(float*)svar->Data, separator);
            break;

        case Double:
            printf("%.15g%s", *(double*)svar->Data, separator);
            break;

        case LDouble:
            #ifdef _WIN32
                __mingw_printf("%.19Lg%s", *(long double*)svar->Data, separator);
            #else
                printf("%.19Lg%s", *(long double*)svar->Data, separator);
            #endif // _WIN32
            break;

        case String:
            printf("%s%s", (char*)svar->Data, separator);
            break;

        default:
            fprintf(stderr, "Error: this data type is not supported.\n");
            break;
    }
}


/* Support Functions */


/** \brief Instantiates a DynamicVariable to the value and data type specified by the user.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[out] svar Pointer to the DynamicVariable.
 * \param[in] data Pointer to the specified data.
 * \param[in] DType The type of data to be stored.
 * \retval struct DynamicVariable
 * \arg Returns a DynamicVariable with the specified value assigned to it.
 */
static struct DynamicVariable __Dynvar_NewVar (struct DynamicVariable* svar, void* data, enum DataType DType)
{
    switch (DType)
    {
        case Char:
            svar->Data = malloc(sizeof(signed char));
            *(signed char*)svar->Data = *(signed char*)data;
            svar->Type = Char;
            svar->Size = sizeof(signed char);
            break;

        case Short:
            svar->Data = malloc(sizeof(short));
            *(short*)svar->Data = *(short*)data;
            svar->Type = Short;
            svar->Size = sizeof(short);
            break;

        case Int:
            svar->Data = malloc(sizeof(int));
            *(int*)svar->Data = *(int*)data;
            svar->Type = Int;
            svar->Size = sizeof(int);
            break;

        case Long:
            svar->Data = malloc(sizeof(long));
            *(long*)svar->Data = *(long*)data;
            svar->Type = Long;
            svar->Size = sizeof(long);
            break;

        case LLong:
            svar->Data = malloc(sizeof(long long));
            *(long long*)svar->Data = *(long long*)data;
            svar->Type = LLong;
            svar->Size = sizeof(long long);
            break;

        case UChar:
            svar->Data = malloc(sizeof(unsigned char));
            *(unsigned char*)svar->Data = *(unsigned char*)data;
            svar->Type = UChar;
            svar->Size = sizeof(unsigned char);
            break;

        case UShort:
            svar->Data = malloc(sizeof(unsigned short));
            *(unsigned short*)svar->Data = *(unsigned short*)data;
            svar->Type = UShort;
            svar->Size = sizeof(unsigned short);
            break;

        case UInt:
            svar->Data = malloc(sizeof(unsigned int));
            *(unsigned int*)svar->Data = *(unsigned int*)data;
            svar->Type = UInt;
            svar->Size = sizeof(unsigned int);
            break;

        case ULong:
            svar->Data = malloc(sizeof(unsigned long));
            *(unsigned long*)svar->Data = *(unsigned long*)data;
            svar->Type = ULong;
            svar->Size = sizeof(unsigned long);
            break;

        case ULLong:
            svar->Data = malloc(sizeof(unsigned long long));
            *(unsigned long long*)svar->Data = *(unsigned long long*)data;
            svar->Type = ULLong;
            svar->Size = sizeof(unsigned long long);
            break;

        case Float:
            svar->Data = malloc(sizeof(float));
            *(float*)svar->Data = *(float*)data;
            svar->Type = Float;
            svar->Size = sizeof(float);
            break;

        case Double:
            svar->Data = malloc(sizeof(double));
            *(double*)svar->Data = *(double*)data;
            svar->Type = Double;
            svar->Size = sizeof(double);
            break;

        case LDouble:
            svar->Data = malloc(sizeof(long double));
            *(long double*)svar->Data = *(long double*)data;
            svar->Type = LDouble;
            svar->Size = sizeof(long double);
            break;

        case String:
            svar->Type = String;
            svar->Size = strlen(data) + 1;
            svar->Data = malloc(svar->Size);
            strcpy(svar->Data, data);
            break;

        case Custom:
            svar->Data = malloc(svar->Size);
            memcpy(svar->Data, data, svar->Size);
            svar->Type = Custom;
            break;

        default:
            fprintf(stderr, "Error: this data type is not supported.\n");
            break;
    }

    return *svar;
}
