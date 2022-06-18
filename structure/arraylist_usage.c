#include "arraylist.h"

int main(int argc, char** argv) {
    List *a = initializeWithCapacity(3);

    printf("\t Init len = %d capacity = %d \n", len(a), cap(a));
    
    append(a, 5);
    append(a, 4);
    append(a, 15);
    append(a, 9);
    append(a, 19);
    
    printf("\t New len = %d capacity = %d \n", len(a), cap(a));
    printf("\t a[3] = %d \n", get(a, 3));
    set(a, 3, 8);
    printf("\t a[3] = %d after changed \n", get(a, 3));
}