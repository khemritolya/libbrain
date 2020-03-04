# libbrain

[![Build Status](https://travis-ci.com/khemritolya/libbrain.svg?branch=master)](https://travis-ci.com/khemritolya/libbrain)
 
A header-only library providing brainfsck/c++ interoperability and compile-time brainfsck compilation. You can build the library, or simply paste the header into your project.

## Quick Start

```cpp

#include <cstdio>
#include "libbrain.h"

int main() {
    // allocate some amount of memory for our program to work with
    auto* data = BRAIN_ALLOC_TAPE(10000);

    data[0] = 100;

    // compile a program which moves the value at a cell to a cell
    // 2 cells to the right.
    BRAIN_PROGRAM move = BRAIN_COMPILE([->>+<<]);

    // run the program, get the status code
    int status_code = move.exec(data, 0);

    printf("Finished with status code %i\n", status_code);
    printf("Value at cell 0: %i, 2: %i\n", data[0], data[2]);
}

```

## Building with Tests

##### Requires:
   * gcc-7 or later
   * cmake 3.12 or later

```bash
cmake CMakeLists.txt
cmake  --build .
```
