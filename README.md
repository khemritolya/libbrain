# libbrain

[![Build Status](https://travis-ci.com/khemritolya/libbrain.svg?branch=master)](https://travis-ci.com/khemritolya/libbrain)
 
A header-only library providing brainfsck/C++ interoperability and compile-time brainfsck compilation and embedding. To use, simply point your compiler to the libbrain.h header, or paste it into your project.

**Author**: Luis Hoderlein

**License:** MIT

## Quick Start

```cpp
#include <cstdio>
#include "libbrain.h"

int main() {
    // allocate some amount of memory for our program to work with
    auto* data = BRAIN_ALLOC_TAPE(10000);

    // compiles a program which prints "hello world!"
    // thank you KSab https://codegolf.stackexchange.com/questions/55422/hello-world/163590#163590
    BRAIN_PROGRAM hello_bf = BRAIN_COMPILE(+[>>>->-[>->----<<<]>>]>.---.>+..+++.>>.<.>>---.<<<.+++.------.<-.>>+.);

    // run the program
    // identical to `hello_bf.exec(data, 0, stout, stdin);`
    int status_code = hello_bf.exec(data, 0);
    
    delete[] data;
    
    return status_code;
}
```

## Building

##### Requires:
   * C++17 or later
   * g++-7 or later

Note: libbrain currently uses a GCC exclusive feature (though I've heard clang might also have it) called a ["literal operator template"] in order to turn string literals into brainfsck programs. I have tried to come up with alternatives to this, but have not succeeded. If you have *please* submit a pull request.

["literal operator template"]: https://en.cppreference.com/w/cpp/language/user_literal

##### Building tests and examples

```bash
cmake CMakeLists.txt
cmake  --build .
```

## The How

**The short answer:**
 
 A mixture of C++ [Constexprs] and [Template Metaprogramming].

[Constexprs]: https://en.cppreference.com/w/cpp/language/constexpr
[Template Metaprogramming]: https://en.wikipedia.org/wiki/Template_metaprogramming

**The longer answer:**

The `BRAIN_COMPILE(...)` macro expands to a literal representing a program, using the aforementioned literal operator template. A program is a templated struct which is generic over its state. This struct has an inline function to be expanded at compile time by gcc, which corresponds to a brainfsck operation. Partial template specialization is then used to allow gcc compile the current instruction and to then repeat this process for subsequent instructions.

**The long answer:**
 
For now, just look in [libbrain.h]. Pay attention to `op_at<...>`, `scan_forward<...>` and `scan_backward<...>`.

[libbrain.h]: https://github.com/khemritolya/libbrain/blob/master/libbrain.h

## File IO with libbrain

libbrain uses c-style IO. Sorry about that. This means that when executing a program, you can specify FILE*s for whatever input and output method you want to use. By default (i.e. when not specified), libbrain runs programs using c stdout and stdin. See the example and test directories if you are particularly curious.

## A Warning:

libbrain offers no compile time safety  for pointers. If you move the data pointer outside of the range of the tape that you have specified, that is **UNDEFINED BEHAVIOR** and you will suffer the same consequences (e.g. SIGSEGV) that you would in regular C++.