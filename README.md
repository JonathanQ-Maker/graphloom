GraphLoom - A threaded tensor library
------------------------------------------

### Description
GraphLoom is a math library written in C++ targeting machine learning applications. It aims to achieve maximum utilization of hardware resources by parallelize as much as possible.

### Install
To install GraphLoom run the following commands
```
cmake -G "MinGW Makefiles" . -B build
cmake --build build
cmake --install build --prefix <install-path>/graphloom
```