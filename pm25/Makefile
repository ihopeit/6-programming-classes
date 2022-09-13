all: pm25_sort_cmd pm25_service apr_array_demo rest_service

CFLAGS   = -ggdb -Wall
INCLUDE = -I /opt/homebrew/Cellar/apr/1.7.0_3/include/apr-1 
LIBDIR = -L /opt/homebrew/Cellar/apr/1.7.0_3/lib 
LDFLAGS = -lapr-1 

REST_INCLUDE = -I /opt/homebrew/Cellar/gnutls/3.7.7/include \
    -I /opt/homebrew/Cellar/jansson/2.14/include \
	-I /opt/homebrew/Cellar/libmicrohttpd/0.9.75/include -I /usr/local/include
REST_LIBDIR = -L /opt/homebrew/Cellar/gnutls/3.7.7/lib -L /usr/local/lib
REST_LDFLAGS = -lorcania -lyder -lulfius

apr_array_demo:
	gcc $(CFLAGS) -o apr_array_demo.out apr_array_demo.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)

pm25_sort_cmd: pm25_month_sort.c pm25.h
	gcc $(CFLAGS) -o pm25_month_sort.out pm25_month_sort.c

pm25_service: pm25_service.c pm25.h ../common.h
	gcc $(CFLAGS) -o pm25_service.out pm25_service.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)

rest_service: rest_service.c rest_service.h
	gcc $(CFLAGS) -o rest_service.out rest_service.c $(REST_LDFLAGS) $(REST_INCLUDE) $(REST_LIBDIR)

clean:
	-rm *.out
	-rm -rf *.out.dSYM
