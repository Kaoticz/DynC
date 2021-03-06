#include "../DynHeaders/dynlst.h"


/* Private Functions */


static struct __DynlstElem* __Dynlst_NewElem (void*, enum DataType);
static struct __DynlstElem* __Dynlst_GetElem (struct DynlstHeader*, int);
static void __Dynlst_RemoveFirst (struct DynlstHeader*);
static void __Dynlst_ClearElem (struct __DynlstElem*);


/* Public Functions */


/** \brief Searches for the specified data and returns its index position in the list, if it exists.
 * \remark It does not work with dynamic variables and will always return -1 for them.
 * \param[in] header The header of the linked list.
 * \param[in] data The data to be searched for.
 * \param[in] dataSize The size of the specified data, in bytes.
 * \retval int
 * \arg If successful, returns the index position of the specified data.
 * \arg returns -1 if the data is not found in the list.
 */
int DynlstIndexOf (struct DynlstHeader* header, void* data, int dataSize)
{
    struct __DynlstElem* element = header->First;

    // Couldn't get this to work with dynamic vars
    for (int counter = 0; element; counter++, element = element->Next)
    {
        if (dataSize != (int)element->DataSize)
            continue;
        else if (!memcmp(element->Data, data, dataSize))
            return counter;
    }

    return -1;
}

/** \brief Removes the element at the end of the list.
 * \remark It works the same as DynlstPop().
 * \param[in] header The header of the linked list.
 * \return \c void
 */
inline void DynlstDequeue (struct DynlstHeader* header)
{
    DynlstPop(header);
}

/** \brief Removes the element at the specified index.
 * \remark If the specified index is out of range, the function will either
 * remove the first or the last element in the list.
 * \param[in] header The header of the linked list.
 * \param[in] index The index to remove the list element from.
 * \return \c void
 */
void DynlstRemove (struct DynlstHeader* header, int index)
{
    if (index >= (int)header->Count)
    {
        DynlstPop(header);
        return;
    }
    else if (index <= 0)
    {
        __Dynlst_RemoveFirst(header);
        return;
    }

    struct __DynlstElem* toNuke = __Dynlst_GetElem(header, index);

    // Link forward with previous
    toNuke->Next->Prev = toNuke->Prev;
    toNuke->Prev->Next = toNuke->Next;

    // Update header
    header->Count--;
    header->Size -= toNuke->DataSize;
    header->TotalSize = header->TotalSize - toNuke->DataSize - sizeof(__DynlstElem);

    // Nuke the specified element
    __Dynlst_ClearElem(toNuke);
}

/** \brief Adds an element to a linked list at the specified index.
 * \remark If the specified index is out of range, the function will either
 * enqueue or push the element into the list.
 * \param[in] header The header of the linked list.
 * \param[in] index The index to add the list element to.
 * \return \c void
 */
void DynlstAdd (struct DynlstHeader* header, void* data, int index, enum DataType DType)
{
    if (index >= (int)header->Count)
    {
        DynlstPush(header, data, DType);
        return;
    }
    else if (index <= 0)
    {
        DynlstEnqueue(header, data, DType);
        return;
    }

    struct __DynlstElem* newElem = __Dynlst_NewElem(data, DType);   // Create new element
    struct __DynlstElem* atIndex = __Dynlst_GetElem(header, index); // Fetch element at index

    newElem->Next = atIndex;        // Link new element to forward
    newElem->Prev = atIndex->Prev;  // Link new element to previous
    newElem->Prev->Next = newElem;  // Link previous to new element
    atIndex->Prev = newElem;        // Link forward to new element

    // Update header
    header->Count++;
    header->Size += newElem->DataSize;
    header->TotalSize += newElem->DataSize + sizeof(__DynlstElem);

}

/** \brief Overwrites a list element with the provided data at the specified index.
 *
 * \param[in] header The header of the linked list.
 * \param[in] data The data to be written to the list element.
 * \param[in] index The index of the list element to be changed.
 * \param[in] DType The type of data that's being written to the list element.
 * \return \c void
 */
void DynlstSet (struct DynlstHeader* header, void* data, int index, enum DataType DType)
{
    if (index < 0 || index >= (int)header->Count)
    {
        fprintf(stderr, "Error: index out of range.\n");
        return;
    }

    struct __DynlstElem* toNuke = __Dynlst_GetElem(header, index);
    struct __DynlstElem* newElem = __Dynlst_NewElem(data, DType);

    // Safely link newElem to the list
    if (index == 0)
    {
        if (header->Count > 1)
        {
            // If element is not the only one in the list
            // Link newElem to the next element
            newElem->Next = toNuke->Next;

            // Link next element to newElem
            toNuke->Next->Prev = newElem;
        }
        else
        {
            // If element is the only one in the list
            header->Last = newElem;
        }

        // Update header
        header->First = newElem;
    }
    else if (index == (int)header->Count - 1)
    {
        // If element is the last in the list
        // Link newElem to the second to last element
        newElem->Prev = toNuke->Prev;

        // Link second to last to newElem
        toNuke->Prev->Next = newElem;

        // Update header
        header->Last = newElem;
    }
    else
    {
        // If element is in the middle of the list
        // Link list to newElem
        toNuke->Prev->Next = newElem;
        toNuke->Next->Prev = newElem;

        // Link newElem to list
        newElem->Next = toNuke->Next;
        newElem->Prev = toNuke->Prev;
    }

    // Update the header
    header->Size = (header->Size - toNuke->DataSize) + newElem->DataSize;
    header->TotalSize = (header->TotalSize - toNuke->DataSize) + newElem->DataSize;

    // Nuke the old element
    __Dynlst_ClearElem(toNuke);
}

/** \brief Returns a pointer to the list element at the specified index.
 * \remark It's recommended that the members of the list element be
 * accessed for reading purposes only.
 * \param[in] header The header of the linked list.
 * \param[in] index The index to fetch the list element from.
 * \retval struct __DynlstElem*
 * \arg If successful, returns a pointer to the list element.
 * \arg Otherwise, returns NULL.
 */
struct __DynlstElem* DynlstGet (struct DynlstHeader* header, int index)
{
    if (index < 0 || index >= (int)header->Count)
    {
        fprintf(stderr, "Error: index out of range.\n");
        return NULL;
    }

    return __Dynlst_GetElem(header, index);
}

/** \brief Deallocates the memory used by the linked list and resets all its properties.
 *
 * \param[in] header The header of the linked list.
 * \return \c void
 */
void DynlstClear (struct DynlstHeader* header)
{
    struct __DynlstElem* element = header->Last->Prev;

    // Clear the list
    while (element)
    {
        __Dynlst_ClearElem(element->Next);
        element = element->Prev;
    }

    // Clear the remaining first element of the list
    __Dynlst_ClearElem(header->First);

    // Reset the header
    header->First = NULL;
    header->Last = NULL;
    header->Count = 0;
    header->Size = 0;
    header->TotalSize = sizeof(Dynlst);
}

/** \brief Removes the last list element of a linked list.
 *
 * \param[in] header The header of the linked list.
 * \return \c void
 */
void DynlstPop (struct DynlstHeader* header)
{
    // Check if the list is empty
    if (!header->First)
        return;

    // Temporarily store address of the last element
    struct __DynlstElem* toNuke = header->Last;

    // If element is not the only one in the list
    if (header->Count > 1)
    {
        header->Last = toNuke->Prev;    // Use it to get to the second to last element. Set header->Last to it
        header->Last->Next = NULL;      // Set its Next pointer to NULL
    }
    else
    {
        header->First = NULL;           // If element is the only one in the list,
        header->Last = NULL;            // reset the header pointers
    }

    // Update header
    header->Count--;
    header->Size -= toNuke->DataSize;
    header->TotalSize -= toNuke->DataSize + sizeof(__DynlstElem);

    // Deallocate the element
    __Dynlst_ClearElem(toNuke);
}

/** \brief Adds a list element to the end of a linked list.
 *
 * \param[in] header The header of the linked list.
 * \param[in] data The data to be added.
 * \param[in] DType The type of data to be added.
 * \return \c void
 */
void DynlstPush (struct DynlstHeader* header, void* data, enum DataType DType)
{
    struct __DynlstElem* newElem = __Dynlst_NewElem(data, DType);

    // Exit if allocation for the new list element has failed
    if (!newElem)
        return;

    // Update the list header
    // If this is the first element in the list, link it to the header
    if (header->Count == 0)
    {
        header->First = header->Last = newElem;
    }
    else
    {
        newElem->Prev = header->Last;       // Link oldElem to newElem
        header->Last->Next = newElem;       // Link newElem to oldElem
        header->Last = newElem;             // Link header->Last to newElem
    }

    header->Count++;                                                  // Update header count
    header->Size += newElem->DataSize;                                // Update amount of bytes allocated for the data stored in the list
    header->TotalSize += newElem->DataSize + sizeof(__DynlstElem);    // Update total amount of bytes in the list (data + objects)
}

/** \brief Adds a list element to the beginning of a linked list.
 *
 * \param[in] header The header of the linked list.
 * \param[in] data The data to be added.
 * \param[in] DType The type of data to be added.
 * \return \c void
 */
void DynlstEnqueue (struct DynlstHeader* header, void* data, enum DataType DType)
{
    struct __DynlstElem* newElem = __Dynlst_NewElem(data, DType);

    // Exit if allocation for the new list element has failed
    if (!newElem)
        return;

    // Update the list header
    // If this is the first element in the list, link it to the header
    if (header->Count == 0)
    {
        header->First = header->Last = newElem;
    }
    else
    {
        header->First->Prev = newElem;      // Link oldElem to the newElem
        newElem->Next = header->First;      // Link newElem to oldElem
        header->First = newElem;            // Link header->First to newElem
    }

    header->Count++;                                                // Update header count
    header->Size += newElem->DataSize;                              // Update amount of bytes allocated for the data stored in the list
    header->TotalSize += newElem->DataSize + sizeof(__DynlstElem);    // Update total amount of bytes in the list (data + objects)
}

/** \brief Prints all the data in a linked list and its stats.
 *
 * \param[in] header The header of the linked list.
 * \param[in] delimiter A string to be added between each list element.
 * \return \c void
 */
void DynlstPrintAll (struct DynlstHeader* header, char* delimiter)
{
    printf("Elements: ");
    DynlstPrint(header, delimiter);

    printf(
        "\nCount = %u"
        "\nSize = %u bytes"
        "\nTotalSize = %u bytes"
        "\n\n", header->Count, header->Size, header->TotalSize
    );
}

/** \brief Prints all the data in a linked list.
 *
 * \param[in] header The header of the linked list.
 * \param[in] delimiter A string to be added between each list element.
 * \return \c void
 */
void DynlstPrint (struct DynlstHeader* header, char* delimiter)
{
    struct __DynlstElem* element = header->First;

    // While element->Next is not NULL
    while (element)
    {
        switch (element->Type)
        {
            case Char:
                printf("%c%s", *(signed char*)element->Data, delimiter);
                break;

            case Short:
                printf("%d%s", *(short*)element->Data, delimiter);
                break;

            case Int:
                printf("%d%s", *(int*)element->Data, delimiter);
                break;

            case Long:
                printf("%ld%s", *(long*)element->Data, delimiter);
                break;

            case LLong:
                #ifdef _WIN32
                    __mingw_printf("%lld%s", *(long long*)element->Data, delimiter);
                #else
                    printf("%lld%s", *(long long*)element->Data, delimiter);
                #endif // _WIN32
                break;

            case UChar:
                printf("%c%s", *(unsigned char*)element->Data, delimiter);
                break;

            case UShort:
                printf("%u%s", *(unsigned short*)element->Data, delimiter);
                break;

            case UInt:
                printf("%u%s", *(unsigned int*)element->Data, delimiter);
                break;

            case ULong:
                printf("%lu%s", *(unsigned long*)element->Data, delimiter);
                break;

            case ULLong:
                #ifdef _WIN32
                    __mingw_printf("%llu%s", *(unsigned long long*)element->Data, delimiter);
                #else
                    printf("%llu%s", *(unsigned long long*)element->Data, delimiter);
                #endif // _WIN32
                break;

            case Float:
                printf("%.6g%s", *(float*)element->Data, delimiter);
                break;

            case Double:
                printf("%.15g%s", *(double*)element->Data, delimiter);
                break;

            case LDouble:
                #ifdef _WIN32
                    __mingw_printf("%.19Lg%s", *(long double*)element->Data, delimiter);
                #else
                    printf("%.19Lg%s", *(long double*)element->Data, delimiter);
                #endif // _WIN32
                break;

            case String:
                printf("%s%s", (char*)element->Data, delimiter);
                break;

            case Var:
                DynvarPrint(element->Data, delimiter);
                break;

            default:
                fprintf(stderr, "Error: this data type is not supported.\n");
                break;
        }

        // Advance to the next list element
        element = element->Next;
    }
}

/** \brief Prints the data stored in a list element at the specified index.
 *
 * \param[in] header The header of the linked list.
 * \param[in] index The index of the element to be printed.
 * \param[in] separator A string to be added after the stored data.
 * It can be empty.
 * \return \c void
 */
void DynlstPrintElem (struct DynlstHeader* header, int index, char* separator)
{
    struct __DynlstElem* element = DynlstGet(header, index);

    // If index is out of range, quit
    if (!element)
        return;

    switch (element->Type)
    {
        case Char:
            printf("%c%s", *(signed char*)element->Data, separator);
            break;

        case Short:
            printf("%d%s", *(short*)element->Data, separator);
            break;

        case Int:
            printf("%d%s", *(int*)element->Data, separator);
            break;

        case Long:
            printf("%ld%s", *(long*)element->Data, separator);
            break;

        case LLong:
            #ifdef _WIN32
                __mingw_printf("%lld%s", *(long long*)element->Data, separator);
            #else
                printf("%lld%s", *(long long*)element->Data, separator);
            #endif // _WIN32
            break;

        case UChar:
            printf("%c%s", *(unsigned char*)element->Data, separator);
            break;

        case UShort:
            printf("%u%s", *(unsigned short*)element->Data, separator);
            break;

        case UInt:
            printf("%u%s", *(unsigned int*)element->Data, separator);
            break;

        case ULong:
            printf("%lu%s", *(unsigned long*)element->Data, separator);
            break;

        case ULLong:
            #ifdef _WIN32
                __mingw_printf("%llu%s", *(unsigned long long*)element->Data, separator);
            #else
                printf("%llu%s", *(unsigned long long*)element->Data, separator);
            #endif // _WIN32
            break;

        case Float:
            printf("%.6g%s", *(float*)element->Data, separator);
            break;

        case Double:
            printf("%.15g%s", *(double*)element->Data, separator);
            break;

        case LDouble:
            #ifdef _WIN32
                __mingw_printf("%.19Lg%s", *(long double*)element->Data, separator);
            #else
                printf("%.19Lg%s", *(long double*)element->Data, separator);
            #endif // _WIN32
            break;

        case String:
            printf("%s%s", (char*)element->Data, separator);
            break;

        case Var:
            DynvarPrint(element->Data, separator);
            break;

        default:
            fprintf(stderr, "Error: this data type is not supported.\n");
            break;
    }
}


/* Support Functions */


/** \brief Instantiates a new list element with the specified data.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] data The data to be added.
 * \param[in] DType The type of data to be added.
 * \retval struct __DynlstElem*
 * \arg If successful, returns a pointer to the newly generated list element.
 * \arg Otherwise, returns NULL.
 */
static struct __DynlstElem* __Dynlst_NewElem (void* data, enum DataType DType)
{
    struct __DynlstElem* element = malloc(sizeof(__DynlstElem));

    if (!element)
    {
        fprintf(stderr, "Error: failed to allocate memory for a new list element.\n");
        return NULL;
    }

    switch (DType)
    {
        case Char:
            element->Data = malloc(sizeof(signed char));
            *(signed char*)element->Data = *(signed char*)data;
            element->Type = Char;
            element->TypeSize = sizeof(signed char);
            element->DataSize = sizeof(signed char);
            break;

        case Short:
            element->Data = malloc(sizeof(short));
            *(short*)element->Data = *(short*)data;
            element->Type = Short;
            element->TypeSize = sizeof(short);
            element->DataSize = sizeof(short);
            break;

        case Int:
            element->Data = malloc(sizeof(int));
            *(int*)element->Data = *(int*)data;
            element->Type = Int;
            element->TypeSize = sizeof(int);
            element->DataSize = sizeof(int);
            break;

        case Long:
            element->Data = malloc(sizeof(long));
            *(long*)element->Data = *(long*)data;
            element->Type = Long;
            element->TypeSize = sizeof(long);
            element->DataSize = sizeof(long);
            break;

        case LLong:
            element->Data = malloc(sizeof(long long));
            *(long long*)element->Data = *(long long*)data;
            element->Type = LLong;
            element->TypeSize = sizeof(long long);
            element->DataSize = sizeof(long long);
            break;

        case UChar:
            element->Data = malloc(sizeof(unsigned char));
            *(unsigned char*)element->Data = *(unsigned char*)data;
            element->Type = UChar;
            element->TypeSize = sizeof(unsigned char);
            element->DataSize = sizeof(unsigned char);
            break;

        case UShort:
            element->Data = malloc(sizeof(unsigned short));
            *(unsigned short*)element->Data = *(unsigned short*)data;
            element->Type = UShort;
            element->TypeSize = sizeof(unsigned short);
            element->DataSize = sizeof(unsigned short);
            break;

        case UInt:
            element->Data = malloc(sizeof(unsigned int));
            *(unsigned int*)element->Data = *(unsigned int*)data;
            element->Type = UInt;
            element->TypeSize = sizeof(unsigned int);
            element->DataSize = sizeof(unsigned int);
            break;

        case ULong:
            element->Data = malloc(sizeof(unsigned long));
            *(unsigned long*)element->Data = *(unsigned long*)data;
            element->Type = ULong;
            element->TypeSize = sizeof(unsigned long);
            element->DataSize = sizeof(unsigned long);
            break;

        case ULLong:
            element->Data = malloc(sizeof(unsigned long long));
            *(unsigned long long*)element->Data = *(unsigned long long*)data;
            element->Type = ULLong;
            element->TypeSize = sizeof(unsigned long long);
            element->DataSize = sizeof(unsigned long long);
            break;

        case Float:
            element->Data = malloc(sizeof(float));
            *(float*)element->Data = *(float*)data;
            element->Type = Float;
            element->TypeSize = sizeof(float);
            element->DataSize = sizeof(float);
            break;

        case Double:
            element->Data = malloc(sizeof(double));
            *(double*)element->Data = *(double*)data;
            element->Type = Double;
            element->TypeSize = sizeof(double);
            element->DataSize = sizeof(double);
            break;

        case LDouble:
            element->Data = malloc(sizeof(long double));
            *(long double*)element->Data = *(long double*)data;
            element->Type = LDouble;
            element->TypeSize = sizeof(long double);
            element->DataSize = sizeof(long double);
            break;

        case String:
            element->Type = String;
            element->TypeSize = sizeof(char);
            element->DataSize = strlen(data) + 1;
            element->Data = malloc(element->DataSize);
            strcpy(element->Data, data);
            break;

        case Var:
            element->Data = malloc(sizeof(Dynvar));
            *(Dynvar*)element->Data = DynvarCopy(data);
            element->Type = Var;
            element->TypeSize = sizeof(Dynvar);
            element->DataSize = sizeof(Dynvar) + ((Dynvar*)element->Data)->Size;
            break;

        default:
            fprintf(stderr, "Error: this data type is not supported.\n");
            return NULL;
    }

    // These pointers should be properly initialized after this call is returned
    element->Next = NULL;
    element->Prev = NULL;

    return element;
}

/** \brief Safely removes the first element of a linked list.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] header The header of the linked list.
 * \return \c void
 */
static void __Dynlst_RemoveFirst (struct DynlstHeader* header)
{
    // If element is the only one in the list
    if (header->Count <= 1)
    {
        DynlstPop(header);
        return;
    }

    struct __DynlstElem* toNuke = header->First;

    header->First = toNuke->Next;
    toNuke->Next->Prev = NULL;

    // Update header
    header->Count--;
    header->Size -= toNuke->DataSize;
    header->TotalSize = header->TotalSize - toNuke->DataSize - sizeof(__DynlstElem);

    // Nuke the element
    __Dynlst_ClearElem(toNuke);
}

/** \brief Deallocates a list element and the data stored in it.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] element The header of the linked list.
 * \return \c void
 */
static void __Dynlst_ClearElem (struct __DynlstElem* element)
{
    if (element->Type == Var)
        DynvarClear(element->Data);

    free(element->Data);
    free(element);
}

/** \brief Returns a pointer to the list element at the specified index.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] header The header of the linked list.
 * \param[in] index The index to fetch the list element from.
 * \retval struct __DynlstElem*
 * \arg Returns a pointer to the list element.
 */
static struct __DynlstElem* __Dynlst_GetElem (struct DynlstHeader* header, int index)
{
    struct __DynlstElem* target;

    if (index <= (int)header->Count / 2)
    {
        // Start from the beginning
        target = header->First;

        for (int counter = 0; counter != index; counter++)
            target = target->Next;
    }
    else
    {
        // Start from the end
        target = header->Last;

        for (int counter = header->Count - 1; counter != index; counter--)
            target = target->Prev;
    }

    return target;
}
