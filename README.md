# Dynamic Collections
### Implementations of data structures in C

This is my personal take on (some) data structures that I did to solidify my knowledge on C and basic computer science principles. I may come back to this eventually when I get burnt out from other languages. I also intend on making it `-pendantic` compliant (as of now, it performs pointer arithmetic on void pointers).  

The data structures implemented here include:  
- The `var` type. It's a struct that encapsulates data of any type.  
- The `Dynarr` type. It's a struct that works as a header for a dynamic array (an array that expands or shrinks according to the program's needs).  
- The `Dynlst` type. It's a struct that works as a header for a double-linked list.  

The `Samples*.c` files demonstrate a few of the functionalities of the library.

Made on Windows 10, with GCC 9.2.0.
Written on CodeBlocks. Documentation was supplied by Doxygen.
  
## How to compile
- Compile the library (flags are optional):  
```
gcc -Wall -g -Winline -Wunreachable-code -Wmain -pedantic  -c dynarr.c -o dynarr.o
```
```
gcc -Wall -g -Winline -Wunreachable-code -Wmain -pedantic  -c dynlst.c -o dynlst.o
```
```
gcc -Wall -g -Winline -Wunreachable-code -Wmain -pedantic  -c dynvar.c -o dynvar.o
```
- Compile your program (flags are optional):
```
gcc -Wall -g -Winline -Wunreachable-code -Wmain -pedantic  -c main.c -o main.o
```
- Link everything together:
```
gcc -o main.exe dynarr.o dynlst.o dynvar.o main.o   
```



