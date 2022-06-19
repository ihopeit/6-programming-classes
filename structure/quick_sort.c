// quick sort implementation
#include <stdio.h>

void swap(int* x, int* y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
 
/* 把高位元素作为基准值，找到一个位置，把基准值放到这个位置，
使得比基准值小的数放到左边；比基准值大的数字放到右边; 返回这个位置 */
int partition (int arr[], int low, int high){
    int pivot = arr[high]; // pivot 基准值

    // 位置 i 左侧的元素都比基准值小，最后把 pivot 移动到 i+1的位置
    int i = (low - 1); 
 
    for (int j = low; j <= high - 1; j++){
        // 找到一个比基准值小的值，索引往前加 1;
        // 如果 arr[j] > pivot，则不操作，
        // 此时 i 值即为当前 j 的位置，j的指针继续右移，
        // 等到找到右侧的小值跟 j 互换
        if (arr[j] < pivot){
            i++; // i+1，留一个位置存储这个小值， j 跟 i 互换:
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]); // pivot 移动到 i+1 位置
    return (i + 1);
}
 
void printArray(int arr[], int size){
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

/* arr[] --> 待排序数组,
   low  -->  起始位置,
   high --> 结束位置 */
void quickSort(int arr[], int low, int high){
    if (low >= high)
        return;

    /* pi 是分割位置, 将基准点移动到 pi 点，
    使得 pi 左边的数比基准点小，右边的数比基准点大 */
    int pi = partition(arr, low, high);
 
    // 分别排序左右两个分区的元素
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
}
 
int main(){
    int arr[] = {5, 9, 10, 3, 4, 7};
    int n = sizeof(arr) / sizeof(arr[0]);
    quickSort(arr, 0, n - 1);
    printf("Sorted array: ");
    printArray(arr, n);
    return 0;
}