### C implementation of a dynamic array similar to the Java ArrayList.

* ANSI / C89 code
* Compiler independent
* CPU Cache friendly
* 32-/64-bit compatible
* Fully documented with [Doxygen](http://www.doxygen.org)
* Unit tested with [sput](http://www.lingua-systems.com/unit-testing/) framework
* Valgrind memory leak checked

If you send the array across the network or write it to a binary file and send it to another process on a different computer it may produce garbage on the other end because this implementation uses size_t which may not be equal in bytes on different machines.
