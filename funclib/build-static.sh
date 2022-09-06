# 编译产生 hello.o, sum.o 的对象文件（-c 参数指定编译文件）
gcc -c hello.c sum.c 

#  ar 打包成 libhello.a 存档文件
ar rcs libhello.a hello.o sum.o 

# 引用静态库编译出可执行文件（gcc 默认会执行编译，链接两个步骤）：
gcc main.c libhello.a

# 也可以指定库文件的目录， 以及库文件的名称来编译
gcc main.c -L. -lhello
