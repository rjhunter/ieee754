# README #

This is a simple app for exploring the structure of IEEE 754 floating point numbers.  A single floating point number is presented as a sequence of "bit buttons" that can be manipulated in several ways.  The resulting floating point value, as well as various meta-data are updated after every operation.

### What is this repository for? ###

* Learn about floating point numbers!
* Version 0.01 (alpha)

### How do I get set up? ###

To build this application, you will need the following:

* c++11 compiler
* gtkmm-3.0
* cmake 3.2

Here is an example build on a unix system (starting in the top-level directory of the project):

```
$ mkdir build
$ cd build/
$ /usr/local/bin/cmake .. -G "Unix Makefiles"
-- The C compiler identification is GNU 4.9.2
-- The CXX compiler identification is GNU 4.9.2
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found PkgConfig: /usr/bin/pkg-config (found version "0.28") 
-- checking for module 'gtkmm-3.0'
--   found gtkmm-3.0, version 3.14.0
-- Configuring done
-- Generating done
-- Build files have been written to: /xxx/xxx/xxx/build
$ make
[ 50%] Building CXX object CMakeFiles/ieee754.dir/src/Ieee754.cpp.o
[100%] Building CXX object CMakeFiles/ieee754.dir/src/main.cpp.o
Linking CXX executable ieee754
[100%] Built target ieee754
$ cp ../src/ieee754.glade .
$ ./ieee754 
```
