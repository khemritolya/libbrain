#include <cstdio>
#include "libbrain.h"

int main() {
    // allocate some amount of memory for our program to work with
    auto* data = BRAIN_ALLOC_TAPE(10000);

    // compiles a program which prints "hello world!"
    // thank you KSab https://codegolf.stackexchange.com/questions/55422/hello-world/163590#163590
    BRAIN_PROGRAM hello_bf = BRAIN_COMPILE(+[>>>->-[>->----<<<]>>]>.---.>+..+++.>>.<.>>---.<<<.+++.------.<-.>>+.);

    // run the program
    // identical to `hello_bf.exec(data, 0, stdout, stdin);`
    int status_code = hello_bf.exec(data, 0);

    delete[] data;

    return status_code;
}