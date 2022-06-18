#include <stdio.h>

int main () {
   int a[5] = {10, 8, 15, 6, 9};
   int *p;
   p = a; // 数组名称 a 被转换成指向第一个元素的指针
   p = &a[0]; // 等效于 p = a，指向数组 a 的第一个元素
   p[2] = 12; //x[y] 表示指针 x 之后的第 y 个元素， 等同于 *(x + y)
 
   printf("first element, a[0] = %d, *p = %d \n", a[0], *p);
   printf("a[2] = %d, p[2] = %d \n", a[2], p[2]);
   printf("*(p + 2) = %d, *(a + 2) = %d \n", *(p + 2), *(a + 2));
   printf("sizeof(p):%lu, sizeof(a):%lu\n", sizeof(p), sizeof(a));
}