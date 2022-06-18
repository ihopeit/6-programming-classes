#include <stdio.h>

int main () {

   int a[6] = {10, 8, 15, 6, 9, 3};
   int *p;
   int i;

   p = a;
 
   printf( "Array values using pointer\n");
   for ( i = 0; i < 6; i++ ) {
      printf("*(p + %d) : %d\n",  i, *(p + i) );
   }

   printf( "Array values using a as address\n");
   for ( i = 0; i < 6; i++ ) {
      printf("*(a + %d) : %d\n",  i, *(a + i) );
   }
   return 0;
}