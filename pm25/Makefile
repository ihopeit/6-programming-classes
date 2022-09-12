all: pm25_sort_cmd pm25_service 

CFLAGS   = -ggdb -Wall
INCLUDE = -I /opt/homebrew/Cellar/apr/1.7.0_3/include/apr-1
LIBDIR = -L /opt/homebrew/Cellar/apr/1.7.0_3/lib
LDFLAGS = -lapr-1

pm25_sort_cmd:
	gcc $(CFLAGS) -o pm25_month_sort.out pm25_month_sort.c

pm25_service:
	gcc $(CFLAGS) -o pm25_service.out pm25_service.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)

clean:
	-rm *.out
	-rm -rf *.out.dSYM
