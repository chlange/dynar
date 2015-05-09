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

##### Functions 

* daAppend
* daClear
* daClone
* daContains
* daCreate
* daDestroy
* daDump
* daErrToString
* daGet
* daGetFirst
* daGetLast
* daIncrease
* daIndexOf
* daInsertAt
* daIsEmpty
* daLastIndexOf
* daPrepend
* daRemove
* daRemoveDirty
* daRemoveRange
* daSet
* daSize

##### Documentation
Generate the HTML documentation with doxygen or refer to the plain text documentation in the header file.
```
$ doxygen Doxyfile
```

##### Build
Build the object code with the obj target of the makefile.
```
$ make obj
```

Build the static library with the lib target of the makefile.
```
$ make lib
```
##### Test
Use the test target of the makefile to build all tests under the test/ directory.
```
$ make test
```
##### Interoperability
If you send the array across the network or write it to a binary file and send it to another process on a different computer it may produce garbage on the other end because this implementation uses size_t which may not be equal in bytes on different machines.
