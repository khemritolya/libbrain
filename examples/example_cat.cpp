#include <cstdio>
#include "libbrain.h"

int main() {
    auto* data = BRAIN_ALLOC_TAPE(10000);

    // the cat program: reads from the input until such a time as it reads a 'tab' character
    BRAIN_PROGRAM stdin_cat = BRAIN_COMPILE(+++++++++>,<[->-<]>[+++++++++.<+++++++++>,<[->-<]>]);

    int status_code = stdin_cat.exec(data, 0);

    delete[] data;

    return status_code;
}
