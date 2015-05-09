## C implementation of a dynamic array similar to the Java ArrayList.

##### Features
* ANSI / C89 / C90 standard conform code
* Compiler independent code
* 32-/64-bit compatible
* CPU Cache friendly
* [Valgrind](http://www.valgrind.org) memory leak checked
* Unit tested and system tested with [sput](http://www.lingua-systems.com/unit-testing/) framework
* Fully documented with [Doxygen](http://www.doxygen.org)
* MIT license

##### Function overview

* daCreate(DaDesc *desc, int *err)

* daDestroy(DaStruct *da, int *err)

* daSize(DaStruct *da, int *err, size_t *size)

* daIsEmpty(DaStruct *da, int *err)

* daPrepend(DaStruct *da, int *err, const void *element)

* daAppend(DaStruct *da, int *err, const void *element)

* daInsertAt(DaStruct *da, int *err, const void *element, size_t pos)

* daRemove(DaStruct *da, int *err, size_t pos)

* daRemoveDirty(DaStruct *da, int *err, size_t pos)

* daRemoveRange(DaStruct *da, int *err, size_t from, size_t to)

* daGet(DaStruct *da, int *err, size_t pos)

* daGetFirst(DaStruct *da, int *err)

* daGetLast(DaStruct *da, int *err)

* daSet(DaStruct *da, int *err, const void *element, size_t pos)

* daIncrease(DaStruct *da, int *err, size_t n, int mode)

* daClear(DaStruct *da, int *err, int mode)

* daContains(DaStruct *da, int *err, const void *element)

* daIndexOf(DaStruct *da, int *err, const void *element, size_t *index)

* daLastIndexOf(DaStruct *da, int *err, const void *element, size_t *index)

* daClone(const DaStruct *da, int *err)

* daErrToString(int err)

* daDump(DaStruct *da, int *err)

##### Documentation
Generate the HTML documentation with doxygen or refer to the plain text documentation in the header file.
```
doxygen Doxyfile
```

##### Build
Build the object code with the obj target of the makefile.
```
make obj
```

Build the static library with the lib target of the makefile.
```
make lib
```
##### Test
Use the test target of the makefile to build all tests under the test/ directory.
```
make test
```
##### Interoperability
If you send the array across the network or write it to a binary file and send it to another process on a different computer it may produce garbage on the other end because this implementation uses size_t which may not be equal in bytes on different machines.
