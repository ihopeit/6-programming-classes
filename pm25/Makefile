all: pm25_sort_cmd pm25_service apr_array_demo

CFLAGS   = -ggdb -Wall
INCLUDE = -I /opt/homebrew/Cellar/apr/1.7.0_3/include/apr-1
LIBDIR = -L /opt/homebrew/Cellar/apr/1.7.0_3/lib
LDFLAGS = -lapr-1

apr_array_demo:
	gcc $(CFLAGS) -o apr_array_demo.out apr_array_demo.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)
	
pm25_sort_cmd:
	gcc $(CFLAGS) -o pm25_month_sort.out pm25_month_sort.c

pm25_service:
	gcc $(CFLAGS) -o pm25_service.out pm25_service.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)


clean:
	-rm *.out
	-rm -rf *.out.dSYM
