#include "../DynHeaders/dynarr.h"


/* Private Variables */

/** \brief  This variable keeps track of the data type of the DynamicArray that's currently being
 * sorted through DynarrSort() or DynarrSortDesc().
 * \warning <u>This is a private global variable and is not meant to be directly accessed!</u>
 */
static unsigned short __dynarr_GlobalComparType = 0;


/* Private Functions */

static void __Dynarr_NewArray (struct DynamicArray*, int, enum DataType);
static int __Dynarr_RemoveElement (struct DynamicArray*, int);
static void __Dynarr_SortArray (struct DynamicArray*, int (*)(const void*, const void*));
static int __Dynarr_CompareByType (const void*, const void*);
static int __Dynarr_CompareByTypeDesc (const void*, const void*);
static int __Dynarr_VarCmp (struct DynamicVariable*, struct DynamicVariable*);


/* Public Functions */


/** \brief Initializes an object of type DynamicArray according to the specified data type.
 *
 * \param[in] initialCapacity The amount of elements the array should initially be able to store.
 * \param[in] DType The data type of the elements to be stored in the array.
 * \retval struct DynamicArray
 * \arg If successful, the DynamicArray will contain a pointer to the allocated array.
 * \arg Otherwise, the DynamicArray will contain a null pointer.
 */
struct DynamicArray NewDynamicArray (int initialCapacity, enum DataType DType)
{
    // If initial capacity is invalid, set it to default
    if (initialCapacity <= 0)
        initialCapacity = 10;

    // Create the struct that's going to be assigned to the calling struct
    struct DynamicArray sarray = { .Count = 0, .Capacity = initialCapacity };

    // Instantiate the right type of array
    __Dynarr_NewArray(&sarray, initialCapacity, DType);

    // If memory allocation went wrong, print a warning on the screen
    if (!sarray.Array)
    {
        fprintf(stderr, "Error: failed to allocate memory for the new dynamic array.\n");
        DynarrClear(&sarray);
    }

    return sarray;
}

/** \brief Adds an element to a DynamicArray.
 *
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] newElement Pointer to the element to be added.
 * \retval unsigned short
 * \arg If successful, returns the amount of elements contained in the array.
 * \arg Otherwise, returns zero.
 */
unsigned short DynarrAdd (struct DynamicArray* sarray, void* newElement)
{
    void* tempPointer = NULL;
    sarray->Count++;

    // If count is going to exceed the capacity, expand the capacity
    if (sarray->Count > sarray->Capacity)
    {
        sarray->Capacity *= 2; // Multiply capacity by 2
        tempPointer = realloc(sarray->Array, sarray->Capacity * sarray->TypeSize);

        if (tempPointer)
        {
            sarray->Array = tempPointer;
        }
        else
        {
            fprintf(stderr, "Error: failed to add an element to the dynamic array.\n");
            return 0;
        }
    }

    // Add the new element to the end of the array
    if (sarray->Type == Var)
    {
        struct DynamicVariable copy = DynvarCopy(newElement);
        memcpy(sarray->Array + sarray->TypeSize * (sarray->Count - 1), &copy, sarray->TypeSize);
    }
    else
    {
        memcpy(sarray->Array + sarray->TypeSize * (sarray->Count - 1), newElement, sarray->TypeSize);
    }

    return sarray->Count;
}

/** \brief Inserts all elements of a DynamicArray into another DynamicArray at the specified index.
 *
 * \param[in] source Pointer to a DynamicArray to be copied from.
 * \param[out] target Pointer to a DynamicArray that will receive the elements.
 * \param[in] targetIndex Index at <i>target</i> where the new elements will be inserted to.
 * \retval int
 * \arg If successful, returns the starting index where the old elements of <i>target</i> have been moved to.
 * \arg Otherwise, returns zero.
 */
int DynarrInsert (struct DynamicArray* source, struct DynamicArray* target, int targetIndex)
{
    return DynarrInsertArray(source->Array, target, source->Count, targetIndex, source->Type);
}

/** \brief Inserts one new element into a DynamicArray at the specified index.
 *
 * \param[in] source Pointer to the data to be copied from.
 * \param[out] target Pointer to a DynamicArray that will receive the element.
 * \param[in] targetIndex Index at <i>target</i> where the new element will be inserted to.
 * \param[in] DType The type of data to be inserted.
 * \retval int
 * \arg If successful, returns the starting index where the old elements of <i>target</i> have been moved to.
 * \arg Otherwise, returns zero.
 */
int DynarrInsertElem (void* source, struct DynamicArray* target, int targetIndex, enum DataType DType)
{
    return DynarrInsertArray(source, target, 1, targetIndex, DType);
}

/** \brief Inserts all elements of a collection into a DynamicArray at the specified index.
 *
 * \param[in] source Pointer to the collection of data to be inserted.
 * \param[out] target Pointer to a DynamicArray that will receive the data.
 * \param[in] sourceAmount Amount of elements to be added.
 * \param[in] targetIndex Index at <i>target</i> where the new elements will be inserted to.
 * \param[in] DType The type of data to be inserted.
 * \retval int
 * \arg If successful, returns the starting index where the old elements of <i>target</i> have been moved to.
 * \arg Otherwise, returns zero.
 */
int DynarrInsertArray (void* source, struct DynamicArray* target, int sourceAmount, int targetIndex, enum DataType DType)
{
    if (DType != target->Type)
    {
        fprintf(stderr, "Error: the dynamic arrays don't store data of the same type.\n");
        return 0;
    }
    else if (targetIndex > target->Count) targetIndex = target->Count;
    else if (targetIndex < 0) targetIndex = 0;

    // Allocate extra memory if the insertion is going to exceed the current space
    if (target->Count + sourceAmount > target->Capacity)
    {
        target->Capacity = target->Count + sourceAmount;
        void* tempPointer = realloc(target->Array, target->Capacity * target->TypeSize);

        if (tempPointer)
        {
            target->Array = tempPointer;
        }
        else
        {
            fprintf(stderr, "Error: failed to add a collection to the dynamic array.\n");
            return 0;
        }
    }

    // Copy old data to its new location in the same array
    // Think of the array as 3 distinct areas: index 0 to targetIndex, sourceAmount and the leftovers to the right (total - tIndex - sAmount)
    memmove(
        target->Array + ((targetIndex + sourceAmount) * target->TypeSize),  // Destination: one index after the last index the source is going to use
        target->Array + (targetIndex * target->TypeSize),                   // Source: starts from the element at the specified index
        (target->Count - targetIndex) * target->TypeSize                    // Copy all elements located to the right of the specified index
    );

    // Copy new data from source to the target
    memcpy(
        target->Array + (targetIndex * target->TypeSize),   // Destination: starts from the specified index of target
        source,                                             // Source: starts from the first index of source
        sourceAmount * target->TypeSize
    );

    // Update the count on the target
    target->Count += sourceAmount;

    return targetIndex + sourceAmount;
}

/** \brief Adds all elements from <i>source</i> to <i>target</i>.
 *
 * \param[in] source Pointer to a DynamicArray to have its data copied from.
 * \param[out] target Pointer to a DynamicArray to have data copied to.
 * \return \c void
 */
void DynarrJoin (struct DynamicArray* source, struct DynamicArray* target)
{
    if (source->Type != target->Type || source->Count == 0)
        return;

    for (int counter = 0; counter < source->Count; counter++)
        DynarrAdd(target, source->Array + (source->TypeSize * counter));
}

/** \brief Returns the index of the specified data in the DynamicArray,
 * if there is one.
 * \param[in] source Pointer to the collection of data to be inserted.
 * \param[in] element The data to be searched for.
 * \retval int
 * \arg Returns the index of the first element that matched the
 * specified data.
 * \arg If the data is not found in the array, returns -1.
 */
int DynarrIndexOf (struct DynamicArray* sarray, void* element)
{
    // This is horrid, but memcmp() is stupid.
    if (sarray->Type == Var)
    {
        for (int counter = 0; counter < sarray->Count; counter++)
        {
            if (!__Dynarr_VarCmp(sarray->Array + (counter * sarray->TypeSize), element))
                return counter;
        }
    }
    else
    {
        for (int counter = 0; counter < sarray->Count; counter++)
        {
            if (!memcmp(sarray->Array + (counter * sarray->TypeSize), element, sarray->TypeSize))
                return counter;
        }
    }

    // Return -1 if data is not found in the array
    return -1;
}

/** \brief Deallocates the memory from the array of a DynamicArray and resets all its properties.
 *
 * \param[out] sarray Pointer to a DynamicArray.
 * \return \c void
 */
void DynarrClear (struct DynamicArray* sarray)
{
    // If the array contains dynamic variables, deallocate each one of them first
    if (sarray->Type == Var)
    {
        for (int counter = 0; counter < sarray->Count; counter++)
            DynvarClear(&((Dynvar*)sarray->Array)[counter]);
    }

    // Deallocate the array
    free(sarray->Array);

    // Reset everything
    sarray->Array = NULL;
    sarray->Capacity = 0;
    sarray->Count = 0;
    sarray->Type = 0;
    sarray->TypeSize = 0;
}

/** \brief Removes an element from a DynamicArray at the specified index.
 * \details This function basically shifts the entire array starting from the provided index.
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] index Index of the element to be removed.
 * \retval int
 * \arg If successful, returns the amount of elements that got shifted.
 * \arg Otherwise, returns zero.
 */
int DynarrRemove (struct DynamicArray* sarray, int index)
{
    if (index > sarray->Count - 1)
    {
        fprintf(stderr, "Error: index out of range.\n");
        return 0;
    }

    // Remove the element at the specified index
    int shifted = __Dynarr_RemoveElement(sarray, index);
    sarray->Count--;

    return shifted;
}

/** \brief Removes all elements specified by the <i>compar</i> function from a DynamicArray.
 *
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] compar
 * \parblock
 * Pointer to a function that takes a void pointer as parameter and returns an \c int.
 * <br> The void pointer represents an element in the DynamicArray.
 * <br> The function must return zero for <u>false</u> evaluations and non-zero values for <u>true</u> evaluations.
 * \endparblock
 * \retval int
 * \arg Returns the amount of elements that got removed.
 */
int DynarrRemoveCustom (struct DynamicArray* sarray, int (*compar)(const void*))
{
    int counter, removed;

    // Remove array elements according to compar's specifications
    for (counter = 0, removed = 0; counter < sarray->Count; counter++)
    {
        if(compar(sarray->Array + (sarray->TypeSize * counter)))
        {
            __Dynarr_RemoveElement(sarray, counter);
            removed++;
            sarray->Count--;
        }
    }

    return removed;
}

/** \brief Deallocates unused memory from a DynamicArray.
 * \remark Multiply the return value by the size of the data type to get the amount of bytes released.
 * \param[out] sarray Pointer to a DynamicArray.
 * \retval int
 * \arg If successful, returns the amount of storage space that got deallocated.
 * \arg Otherwise, returns zero.
 */
int DynarrTrim (struct DynamicArray* sarray)
{
    if (sarray->Capacity == sarray->Count)
        return 0;

    int freed = 0;
    void* tempPointer = realloc(sarray->Array, sarray->Count * sarray->TypeSize);

    if (tempPointer)
    {
        freed = sarray->Capacity - sarray->Count;
        sarray->Array = tempPointer;
        sarray->Capacity = sarray->Count;
    }
    else
    {
        fprintf(stderr, "Error: failed to trim down the dynamic array.\n");
    }

    return freed;
}

/** \brief Expands the memory space allocated to the specified DynamicArray.
 *
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] addSpace Defines how many extra elements the array should be able to store.
 * \return \c void
 */
void DynarrExpand (struct DynamicArray* sarray, int addSpace)
{
    if (addSpace <= 0 || sarray->Type == 0)
        return;

    void* tempPointer = realloc(sarray->Array, sarray->TypeSize * (sarray->Count + addSpace));

    if (tempPointer)
    {
        sarray->Array = tempPointer;
        sarray->Capacity += addSpace;
    }
    else
    {
        fprintf(stderr, "Error: failed to expand the dynamic array.\n");
    }
}

/** \brief Returns a sub-array from a DynamicArray within the range defined by <i>min</i> and <i>max</i>.
 *
 * \param[in] sarray Pointer to a DynamicArray to create a sub-array from.
 * \param[in] min,max Defines index range from which elements should be taken from.
 * \retval struct DynamicArray
 * \arg If successful, returns a DynamicArray that contains a sub-array of <i>sarray</i>.
 * \arg Otherwise, returns a DynamicArray with a null array.
 */
struct DynamicArray DynarrGetSubarray (struct DynamicArray* sarray, int min, int max)
{
    if (min < 0 || max < 0) // If range is below 0, return a null array
    {
        fprintf(stderr, "Error: invalid index range.\n");
        return NewDynamicArray(0, 0);
    }
    else if (min > max)     // If min is greater than max, swap them
    {
        min ^= max;
        max ^= min;
        min ^= max;
    }

    // If max is greater than the array size, set it to the biggest index of the array
    if(max > sarray->Count - 1)
        max = sarray->Count - 1;

    // Instantiate a DynamicArray with enough space to store the values in the range
    struct DynamicArray subArray = NewDynamicArray(max - min + 1, sarray->Type);

    // Add elements within the range to the DynamicArray
    while (min <= max)
        DynarrAdd(&subArray, sarray->Array + (sarray->TypeSize * min++));

    return subArray;
}

/** \brief Returns a sub-array from a DynamicArray specified by the <i>compar</i> function.
 * \remark <u>This function may yield incorrect results for floating-point data types, if
 * your <i>compar</i> function performs mathematical operations on them before they are
 * cast to an integer type.</u>
 * \param[in] sarray Pointer to a DynamicArray to create a sub-array from.
 * \param[in] compar
 * \parblock
 * Pointer to a function that takes a void pointer as parameter and returns an \c int.
 * <br> The void pointer represents an element in the DynamicArray.
 * <br> The function must return zero for <u>false</u> evaluations and non-zero values for <u>true</u> evaluations.
 * \endparblock
 * \retval struct DynamicArray
 * \arg If successful, returns a DynamicArray that contains a sub-array from <i>sarray</i>.
 * \arg Otherwise, returns an empty DynamicArray of the same type as <i>sarray</i>.
 */
struct DynamicArray DynarrGetSubarrayCustom (struct DynamicArray* sarray, int (*compar)(const void*))
{
    int counter;
    struct DynamicArray subArray = NewDynamicArray(10, sarray->Type);

    // Cycle through the array and fetch only the values that meet the compar criteria
    for (counter = 0; counter < sarray->Count; counter++)
    {
        if (compar(sarray->Array + (sarray->TypeSize * counter)))
            DynarrAdd(&subArray, sarray->Array + (sarray->TypeSize * counter));
    }

    // If sub-array is bigger than it should, trim it down.
    if (subArray.Count != subArray.Capacity)
        DynarrTrim(&subArray);

    return subArray;
}

/** \brief Sorts a DynamicArray in ascending order.
 *
 * \param[out] sarray Pointer to a DynamicArray.
 * \return void
 */
void DynarrSort (struct DynamicArray* sarray)
{
    __Dynarr_SortArray(sarray, __Dynarr_CompareByType);
}

/** \brief Sorts a DynamicArray in descending order.
 *
 * \param[out] sarray Pointer to a DynamicArray.
 * \return \c void
 */
void DynarrSortDesc (struct DynamicArray* sarray)
{
    __Dynarr_SortArray(sarray, __Dynarr_CompareByTypeDesc);
}

/** \brief Sorts a DynamicArray in the order specified by the <i>compar</i> function.
 * \remark <u>This function may yield incorrect results for floating-point data types, if
 * your <i>compar</i> function performs mathematical operations on them before they are cast
 * to an integer type.</u>
 * \param[out] sarray: Pointer to a DynamicArray.
 * \param[in] compar
 * \parblock
 * Pointer to a function that compares two values and returns an \c int that is:
 * <br> <u>Less than zero</u>, if first value is going to be placed before the second value.
 * <br> <u>Zero</u>, if both values are to remain side by side.
 * <br> <u>Greater than zero</u>, if first value is going to be placed after the second value.
 * \endparblock
 * \return \c void
 */
void DynarrSortCustom (struct DynamicArray* sarray, int (*compar)(const void*, const void*))
{
    qsort(sarray->Array, sarray->Count, sarray->TypeSize, compar);
}

/** \brief Prints the elements stored in a DynamicArray and its properties.
 *
 * \param[in] sarray Pointer to a DynamicArray.
 * \param[in] delimiter A string to be placed between the elements of the
 * array when they are printed to \c stdout.
 * \return \c void
 */
void DynarrPrintAll (struct DynamicArray* sarray, char* delimiter)
{
    // Print the elements
    printf("Elements: ");
    DynarrPrint(sarray, delimiter);

    // Print the properties
    printf("\nCount: %d\n", sarray->Count);
    printf("Capacity: %d\n", sarray->Capacity);
    #ifdef DEBUG
    printf("Type: %d\n", sarray->Type);             // Debug
    printf("Type Size: %d\n", sarray->TypeSize);    // Debug
    #endif
    printf("Array Size: %d bytes\n\n", sarray->Capacity * sarray->TypeSize);
}

/** \brief Gets the element stored in a DynamicArray at the specified index.
 * \remark It silently quits if the provided index is out of range.
 * \param[in] sarray Pointer to a DynamicArray.
 * \param[in] index The index the specified element is located at.
 * \retval void*
 * \arg If successful, a void pointer to the element at the specified index.
 * \arg Otherwise, a null pointer.
 */
void* DynarrGetElement(struct DynamicArray* sarray, int index)
{
    if (index >= sarray->Count || index < 0)
        return NULL;

    return sarray->Array + (sarray->TypeSize * index);
}

/** \brief Changes the element stored in a DynamicArray at the specified index.
 * \remark It silently quits if the provided index is out of range.
 * \param[in] sarray Pointer to a DynamicArray.
 * \param[out] element Pointer to the new value.
 * \param[in] index The index where the new element will be inserted.
 * \return \c void
 */
void DynarrSetElement(struct DynamicArray* sarray, void* element, int index)
{
    if (index >= sarray->Count || index < 0)
        return;

    memcpy(sarray->Array + (index * sarray->TypeSize), element, sarray->TypeSize);
}

/** \brief Prints the elements stored in a DynamicArray.
 *
 * \param[in] sarray Pointer to a DynamicArray.
 * \param[in] delimiter A string to be placed between the elements of the
 * array when they are printed to \c stdout.
 * \return \c void
 */
void DynarrPrint (struct DynamicArray* sarray, char* delimiter)
{
    int counter;

    switch (sarray->Type)
    {
        case Char:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%c%s", ((signed char*)sarray->Array)[counter], delimiter);
            break;

        case Short:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%d%s", ((short*)sarray->Array)[counter], delimiter);
            break;

        case Int:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%d%s", ((int*)sarray->Array)[counter], delimiter);
            break;

        case Long:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%ld%s", ((long*)sarray->Array)[counter], delimiter);
            break;

        case LLong:
            for (counter = 0; counter < sarray->Count; counter++)
                #ifdef _WIN32
                    __mingw_printf("%lld%s", ((long long*)sarray->Array)[counter], delimiter);
                #else
                    printf("%lld%s", ((long long*)sarray->Array)[counter], delimiter);
                #endif // _WIN32
            break;

        case Float:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%.6g%s", ((float*)sarray->Array)[counter], delimiter);
            break;

        case Double:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%.15g%s", ((double*)sarray->Array)[counter], delimiter);
            break;

        case LDouble:
            for (counter = 0; counter < sarray->Count; counter++)
                #ifdef _WIN32
                    __mingw_printf("%.19Lg%s", ((long double*)sarray->Array)[counter], delimiter);
                #else
                    printf("%.19Lg%s", ((long double*)sarray->Array)[counter], delimiter);
                #endif // _WIN32
            break;

        case UChar:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%c%s", ((unsigned char*)sarray->Array)[counter], delimiter);
            break;

        case UShort:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%u%s", ((unsigned short*)sarray->Array)[counter], delimiter);
            break;

        case UInt:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%u%s", ((unsigned int*)sarray->Array)[counter], delimiter);
            break;

        case ULong:
            for (counter = 0; counter < sarray->Count; counter++)
                printf("%lu%s", ((unsigned long*)sarray->Array)[counter], delimiter);
            break;

        case ULLong:
            for (counter = 0; counter < sarray->Count; counter++)
                #ifdef _WIN32
                    __mingw_printf("%llu%s", ((unsigned long long*)sarray->Array)[counter], delimiter);
                #else
                    printf("%llu%s", ((unsigned long long*)sarray->Array)[counter], delimiter);
                #endif // _WIN32
            break;

        case Var:
            for (counter = 0; counter < sarray->Count; counter++)
                DynvarPrint(&((Dynvar*)sarray->Array)[counter], delimiter);
            break;

        default:
            fprintf(stderr, "Error: attempted to print an unsupported data type or the array is null.\n");
            break;
    }
}


/* Support Functions */


/** \brief This function instantiates a DynamicArray struct of the data type specified by the user.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] elements A number representing how many elements the array should be able to hold.
 * \param[in] DType An enum representing the desired data type.
 * \return \c void
 */
static void __Dynarr_NewArray (struct DynamicArray* sarray, int elements, enum DataType DType)
{
    // Instantiate the right type of array
    switch (DType)
    {
        case Char:
            sarray->Type = Char;
            sarray->TypeSize = sizeof(signed char);
            sarray->Array = calloc(elements, sizeof(signed char));
            break;

        case Short:
            sarray->Type = Short;
            sarray->TypeSize = sizeof(short);
            sarray->Array = calloc(elements, sizeof(short));
            break;

        case Int:
            sarray->Type = Int;
            sarray->TypeSize = sizeof(int);
            sarray->Array = calloc(elements, sizeof(int));
            break;

        case Long:
            sarray->Type = Long;
            sarray->TypeSize = sizeof(long);
            sarray->Array = calloc(elements, sizeof(long));
            break;

        case LLong:
            sarray->Type = LLong;
            sarray->TypeSize = sizeof(long long);
            sarray->Array = calloc(elements, sizeof(long long));
            break;

        case Float:
            sarray->Type = Float;
            sarray->TypeSize = sizeof(float);
            sarray->Array = calloc(elements, sizeof(float));
            break;

        case Double:
            sarray->Type = Double;
            sarray->TypeSize = sizeof(double);
            sarray->Array = calloc(elements, sizeof(double));
            break;

        case LDouble:
            sarray->Type = LDouble;
            sarray->TypeSize = sizeof(long double);
            sarray->Array = calloc(elements, sizeof(long double));
            break;

        case UChar:
            sarray->Type = UChar;
            sarray->TypeSize = sizeof(unsigned char);
            sarray->Array = calloc(elements, sizeof(unsigned char));
            break;

        case UShort:
            sarray->Type = UShort;
            sarray->TypeSize = sizeof(unsigned short);
            sarray->Array = calloc(elements, sizeof(unsigned short));
            break;

        case UInt:
            sarray->Type = UInt;
            sarray->TypeSize = sizeof(unsigned int);
            sarray->Array = calloc(elements, sizeof(unsigned int));
            break;

        case ULong:
            sarray->Type = ULong;
            sarray->TypeSize = sizeof(unsigned long);
            sarray->Array = calloc(elements, sizeof(unsigned long));
            break;

        case ULLong:
            sarray->Type = ULLong;
            sarray->TypeSize = sizeof(unsigned long long);
            sarray->Array = calloc(elements, sizeof(unsigned long long));
            break;

        case Var:
            sarray->Type = Var;
            sarray->TypeSize = sizeof(Dynvar);
            sarray->Array = calloc(elements, sizeof(Dynvar));
            break;

        case Ptr:
            sarray->Type = Ptr;
            sarray->TypeSize = sizeof(void*);
            sarray->Array = calloc(elements, sizeof(void*));
            break;

        default:
            fprintf(stderr, "Error: unrecognized type. A null array was generated.\n");
            sarray->Capacity = 0;
            sarray->Type = 0;
            sarray->TypeSize = 0;
            sarray->Array = NULL;
            break;
    }
}

/** \brief Removes an element from the a DynamicArray at the specified index.
 * \remark This is achieved by shifting the array from the provided index until it
 * reaches the end.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] index The index of the element to be removed.
 * \retval int
 * \arg If successful, returns the amount of shifted elements.
 * \arg Otherwise, returns zero.
 */
static int __Dynarr_RemoveElement (struct DynamicArray* sarray, int index)
{
    int counter;
    void* atIndex = sarray->Array + (index * sarray->TypeSize);
    void* scout = atIndex + sarray->TypeSize;

    switch (sarray->Type)
    {
        case Char:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(signed char), scout += sizeof(signed char))
                *(signed char*)atIndex = *(signed char*)scout;
            break;

        case Short:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(short), scout += sizeof(short))
                *(short*)atIndex = *(short*)scout;
            break;

        case Int:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(int), scout += sizeof(int))
                *(int*)atIndex = *(int*)scout;
            break;

        case Long:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(long), scout += sizeof(long))
                *(long*)atIndex = *(long*)scout;
            break;

        case LLong:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(long long), scout += sizeof(long long))
                *(long long*)atIndex = *(long long*)scout;
            break;

        case Float:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(float), scout += sizeof(float))
                *(float*)atIndex = *(float*)scout;
            break;

        case Double:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(double), scout += sizeof(double))
                *(double*)atIndex = *(double*)scout;
            break;

        case LDouble:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(long double), scout += sizeof(long double))
                *(long double*)atIndex = *(long double*)scout;
            break;

        case UChar:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(unsigned char), scout += sizeof(unsigned char))
                *(unsigned char*)atIndex = *(unsigned char*)scout;
            break;

        case UShort:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(unsigned short), scout += sizeof(unsigned short))
                *(unsigned short*)atIndex = *(unsigned short*)scout;
            break;

        case UInt:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(unsigned int), scout += sizeof(unsigned int))
                *(unsigned int*)atIndex = *(unsigned int*)scout;
            break;

        case ULong:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(unsigned long), scout += sizeof(unsigned long))
                *(unsigned long*)atIndex = *(unsigned long*)scout;
            break;

        case ULLong:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(unsigned long long), scout += sizeof(unsigned long long))
                *(unsigned long long*)atIndex = *(unsigned long long*)scout;
            break;

        case Var:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(Dynvar), scout += sizeof(Dynvar))
                *(Dynvar*)atIndex = *(Dynvar*)scout;
            break;

        case Ptr:
            for (counter = 0; index < sarray->Count - 1; counter++, index++, atIndex += sizeof(void*), scout += sizeof(void*))
                atIndex = scout;
            break;

        default:
            fprintf(stderr, "Error: this data type is not supported.\n");
            counter = 0;
            break;
    }

    return counter;
}

/** \brief This function is a wrapper for the \c qsort() standard function for a simple ascending sort.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[out] sarray Pointer to a DynamicArray.
 * \param[in] compar
 * \parblock
 * Pointer to a function that compares two values of any type and returns an \c int that is:
 * <br> <u>Less than zero</u>, if first value is less than second value.
 * <br> <u>Zero</u>, if first value is equal to second value.
 * <br> <u>Greater than zero</u>, if first value is greater than second value.
 * \endparblock
 * \return \c void
 */
static void __Dynarr_SortArray (struct DynamicArray* sarray, int (*compar)(const void*, const void*))
{
    __dynarr_GlobalComparType = sarray->Type;
    qsort(sarray->Array, sarray->Count, sarray->TypeSize, compar);
}

/** \brief This function is a wrapper for the __Dynarr_CompareByType() function.
 * \remark It takes it's returned value and inverts it by multiplying it to -1.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] first Pointer to the first value.
 * \param[in] second Pointer to the second value.
 * \retval int
 * \arg Returns the output of __Dynarr_CompareByType() multiplied by -1.
 */
static int __Dynarr_CompareByTypeDesc (const void* first, const void* second)
{
    return __Dynarr_CompareByType(first, second) * -1;
}

/** \brief This function is used as a <i>compar</i> function on \c qsort() for simple
 * sortings. It takes two values of any type and subtracts them with each other.
 * \remark If the result is greater than zero, it means that the first value is greater than
 * the second one and vice-versa. If result is equal to zero, both values are equal.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] first Pointer to the first value.
 * \param[in] second Pointer to the second value.
 * \retval int
 * \arg Returns the subtraction between the first and second values.
 */
static int __Dynarr_CompareByType (const void* first, const void* second)
{
    switch (__dynarr_GlobalComparType)
    {
        case Char:
            return *(signed char*)first - *(signed char*)second;

        case Short:
            return *(short*)first - *(short*)second;

        case Int:
            return *(int*)first - *(int*)second;

        case Long:
            return *(long*)first - *(long*)second;

        case LLong:
            return *(long long*)first - *(long long*)second;

        case Float:
            return *(float*)first - *(float*)second;

        case Double:
            return *(double*)first - *(double*)second;

        case LDouble:
            return *(long double*)first - *(long double*)second;

        case UChar:
            return *(unsigned char*)first - *(unsigned char*)second;

        case UShort:
            return *(unsigned short*)first - *(unsigned short*)second;

        case UInt:
            return *(unsigned int*)first - *(unsigned int*)second;

        case ULong:
            return *(unsigned long*)first - *(unsigned long*)second;

        case ULLong:
            return *(unsigned long long*)first - *(unsigned long long*)second;

        case Var:
            // If the variables store data of the same type
            if (*(unsigned short*)(first + sizeof(void*)) == *(unsigned short*)(second + sizeof(void*)))
            {
                // Sort by value
                return memcmp(first, second, *(unsigned short*)(first + sizeof(void*) + sizeof(unsigned short)));
            }
            else // If they don't store data of the same type
            {
                // Sort by type
                return *(unsigned short*)(first + sizeof(void*)) - *(unsigned short*)(second + sizeof(void*));
            }

        default:
            // If data type is not supported, don't do anything
            return 0;
    }
}

/** \brief This function compares the value stored in 2 DynamicVariables with each other.
 * \remark If the result is greater than zero, it means that the first value is greater than
 * the second one and vice-versa. If result is equal to zero, both values are equal.
 * \warning <u>This is a private function and is not meant to be directly invoked!</u>
 * \param[in] first Pointer to the first DynamicVariable.
 * \param[in] second Pointer to the second DynamicVariable.
 * \retval int
 * \arg Returns zero if both values match.
 * \arg Otherwise, returns any non-zero number.
 */
static int __Dynarr_VarCmp (struct DynamicVariable* first, struct DynamicVariable* second)
{
    // This function could be discarded if I reorganize the Dynarr struct and check
    // for the type with basic pointer arithmetic on __Dynarr_CompareByType(), but
    // I can't be bothered
    switch (second->Type)
    {
        // This weird amalgamation is to prevent chars from being matched with
        // strings and vice-versa
        case String:
            if (first->Type == second->Type)
                return strcmp(first->Data, second->Data);

        case Char:
        case UChar:
            if (first->Size != second->Size)
                return -1;

        default: // The ternary is necessary to prevent unallocated bytes from being read in case one type is bigger than another
            return memcmp(first->Data, second->Data, (first->Size < second->Size) ? first->Size : second->Size);
    }
}
