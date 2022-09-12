#include <stdio.h>
#include <stdlib.h>

#include <apr_tables.h>
#include <apr_strings.h>
#define ARRAY_INIT_SZ    32 /*  动态数组的初始大小  */

/* 动态数组使用示例 （TODO 实际代码需要增加错误检查）*/
int main(int argc, const char *argv[]){
    apr_pool_t *mp;
    apr_array_header_t *arr;
        
    apr_initialize();
    apr_pool_create(&mp, NULL);

    /* 创建一个字符串的动态数组 (const char*) */
    arr = apr_array_make(mp, ARRAY_INIT_SZ, sizeof(const char*));

    *(const char**)apr_array_push(arr) = "123"; // 插入数据
    *(const char**)apr_array_push(arr) = "456";
    *(const char**)apr_array_push(arr) = "789";

    /* array iteration， 遍历动态数据中的元素，逐一打印出来 */
    for (int i = 0; i < arr->nelts; i++) {
        const char *s = ((const char**)arr->elts)[i];
        printf("%d: %s\n", i, s);
    }

    /* the array is destroyed when @mp is destroyed */
    apr_pool_destroy(mp);
    
    apr_terminate();
    return 0;
}