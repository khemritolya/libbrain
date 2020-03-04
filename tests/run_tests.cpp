#include <cstdio>
#include "libbrain.h"

#define LIBBRAIN_ASSERT(a, b) a == b ? printf("=> Passed: " #a " == " #b "\n") : printf("=> Failed: " #a " == " #b "\n");

#define LIBBRAIN_TEST printf("\n"); printf(__FUNCTION__); printf("\n");


void move_value_test() {
    LIBBRAIN_TEST

    auto* data = BRAIN_ALLOC_TAPE(10000);
    data[0] = 100;
    data[2] = 76;

    // BF Program which moves the value in one index to one 2 indicies over, overwriting the thing 2 cells over
    BRAIN_PROGRAM move_program = BRAIN_COMPILE(>>[-]<<[->>+<<]);
    int status_code = move_program.exec(data, 0);

    LIBBRAIN_ASSERT(status_code, 0)
    LIBBRAIN_ASSERT(data[0], 0)
    LIBBRAIN_ASSERT(data[1], 0)
    LIBBRAIN_ASSERT(data[2], 100)

    data[1] = 201;

    status_code = move_program.exec(data, 1);
    LIBBRAIN_ASSERT(status_code, 0)
    LIBBRAIN_ASSERT(data[0], 0)
    LIBBRAIN_ASSERT(data[1], 0)
    LIBBRAIN_ASSERT(data[2], 100)
    LIBBRAIN_ASSERT(data[3], 201)

    delete[] data;
}

int main() {
    move_value_test();
}

#undef LIBBRAIN_ASSERT
#undef LIBBRAIN_TEST