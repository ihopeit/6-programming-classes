all: pm25_sort_cmd pm25_service apr_array_demo

CFLAGS   = -ggdb -Wall
INCLUDE = -I /opt/homebrew/Cellar/apr/1.7.0_3/include/apr-1 
LIBDIR = -L /opt/homebrew/Cellar/apr/1.7.0_3/lib 
LDFLAGS = -lapr-1 

apr_array_demo:
	gcc $(CFLAGS) -o apr_array_demo.out apr_array_demo.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)

pm25_sort_cmd: pm25_month_sort.c pm25.h
	gcc $(CFLAGS) -o pm25_month_sort.out pm25_month_sort.c

pm25_service: pm25_service.c pm25.h ../common.h
	gcc $(CFLAGS) -o pm25_service.out pm25_service.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)


clean:
	-rm -f *.out
	-rm -rf *.out.dSYM
