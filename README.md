# array_list
Simple generic dynamic arrays in C using macros and function pointers

Uses the macro `MAKE_LIST_FUN(name, type)` to generate a function `void * alloc_name_list()` that returns a dynamic array for storing `type`s.

The API is modelled on Java's ArrayLists and can be seen in the header file.

This has been more of an experiment in C macros, function pointers and `void *` than an attempt to provide something useful.
That said, the code is simple and works well for storing both raw integers (with some casts) and pointers dynamically.

# License
See LICENSE
