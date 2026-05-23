#include "bool.h"
#include <stdio.h>
#include <assert.h>

int main() {
    bool a = true, b = true, c = false;
    unsigned int d = ((((0 | a) << 1) | b) << 1) | c; // 110

    assert((d >> 2) & 1);

    a = (d >> 2) & 1;
    b = (d >> 1) & 1;
    c = d & 1;


    printf("%d    %d %d %d\n", d, a, b, c);

    return 0;
}
