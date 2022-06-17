#include <stdio.h>
#define PI (3.1415926)

/* 计算圆的周长:
   给定半径，计算并输出圆的周长 */
int main(int argc, char** argv)
{
    float radius = 5.0f;
    float perimeter = 2 * PI * radius;
    printf("Perimeter is: %f \n",perimeter);
    return 0;
}
