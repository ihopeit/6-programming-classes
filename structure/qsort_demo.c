#include <stdio.h>
#include <stdlib.h>
 
// 整数的比较函数， 用作 sort() 函数的排序函数
int compare_ints(const void* a, const void* b){
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}
 
int main(void){
    int ints[] = { -2, 99, 0, -743, 2, -69820, 4 };
    int size = sizeof ints / sizeof *ints;
 
    qsort(ints, size, sizeof(int), compare_ints);
    for (int i = 0; i < size; i++) {
        printf("%d ", ints[i]);
    }
    printf("\n");
}