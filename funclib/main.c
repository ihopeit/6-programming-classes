/* main.c  */
#include <stdio.h>
#include "hello.h"

int main() {
    hello();
    int total = sum(20, 50);
    printf("total: %d", total);
}