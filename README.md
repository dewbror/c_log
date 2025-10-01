C_LOG 0.1.0
===========

C/C++ header-only logging library.

TODO
----
- Fix silent errors...
- Write "Documentation" section in header
- Fix thread safety

Dependencies
------------

  - [cmocka](https://github.com/clibs/cmocka) 1.1.5 (for tests only)

How to Use
----------
In exactly ONE C/C++ file, do this:
```
#define C_LOG_IMPLEMENTATION
#include <c_log.h>
#undef C_LOG_IMPLEMENTATION <-- not strictly necessary
```
This will include all the function prototypes and definitions into that C/C++ file. Including the header without C_LOG_IMPLEMENTATION will only include the function prototypes and NOT their definitions. This is what you do everywhere else you wish to use c_log.h functions in your code.

How to Build and Run Tests (with cmake)
---------------------------------------

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
./Debug/c_log_tests
```

License
-------

The source code in this repository is licensed under the MIT License, see [LICENSE.txt](https://github.com/dewbror/c_log/blob/master/LICENSE.txt). This license applies only to the source code files.

--- end of README ---
