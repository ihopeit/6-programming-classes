OBJECTS = hello.o sum.o

all: $(OBJECTS) static dynamic main

$(OBJECTS): %.o: %.c hello.h
	$(CC) $(LDFLAGS) $(CFLAGS) -c -o $@ $<

static: hello.o sum.o
	#  ar 打包成 libhello.a 存档文件
	ar rcs libhello.a hello.o sum.o

dynamic: hello.c sum.c hello.h
	gcc -shared -fPIC -o libhello.so hello.c sum.c

main: main.c 
	# 引用静态库编译出可执行文件（gcc 默认会执行编译，链接两个步骤）：
	#gcc main.c libhello.a

	# 也可以指定库文件的目录， 以及库文件的名称来编译
	gcc -o main.out main.c -L. -lhello

clean:
	-rm -f *.o *.out *.a *.so
