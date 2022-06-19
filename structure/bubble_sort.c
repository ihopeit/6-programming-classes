// C program for implementation of Bubble sort
#include <stdio.h>
 
int compare_count = 0; // 记录比较次数
void swap(int* x, int* y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
 
void bubbleSort(int arr[], int n){
    int i, j, compared_count = 0;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++){
            compare_count++;
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
}
 
void printArray(int arr[], int size){
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
 
int main(){
    int arr[] = { 5, 1, 4, 2, 8 };
    int n = sizeof(arr) / sizeof(arr[0]);
    bubbleSort(arr, n);
    printf("Compared count: %d, sorted: \n", compare_count);
    printArray(arr, n);
    return 0;
}