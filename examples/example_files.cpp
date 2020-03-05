#include <cstdio>
#include "libbrain.h"

int main() {
    auto* data = BRAIN_ALLOC_TAPE(100);

    // the simple cat program. Simply reads until such a time as it gets code -1 (EOF)
    BRAIN_PROGRAM file_cat = BRAIN_COMPILE(,+[-.,+]);

    // This path works for me. It might not for you. Sorry about that.
    FILE* self = fopen("../../examples/example_files.cpp", "r");

    int status_code = file_cat.exec(data, 0, stdout, self);

    fclose(self);

    delete[] data;

    return status_code;
}