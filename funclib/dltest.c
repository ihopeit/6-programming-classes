#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    void *handle;
    int (*sumfunc)(int a, int b);
    char *error;

    /* 加载之前构建的 libhello.so 程式库*/
    handle = dlopen("./libhello.so", RTLD_LAZY);

    if (!handle){
        fputs(dlerror(), stderr);
        exit(1);
    }

    /* 取得 sum function 的address */
    sumfunc = dlsym(handle, "sum");
    if ((error = dlerror()) != NULL){
        fputs(error, stderr);
        exit(1);
    }

    /* 调用该 function */
    printf("5+10 = %d", sumfunc(5, 10));
    dlclose(handle);
}