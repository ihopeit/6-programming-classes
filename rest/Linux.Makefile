all: rest_service simple_server

CFLAGS   = -ggdb -Wall

REST_INCLUDE = -I /usr/local/include -I /usr/include/apr-1.0/
REST_LIBDIR = -L /usr/local/lib -L /usr/lib/x86_64-linux-gnu/
REST_LDFLAGS = -lorcania -lyder -lulfius -lcurl -lapr-1 -ljansson

rest_service: rest_service.c rest_service.h
        gcc $(CFLAGS) -o rest_service.out rest_service.c $(REST_LDFLAGS) $(REST_INCLUDE) $(REST_LIBDIR)

simple_server: simple_server.c
        gcc $(CFLAGS) -o simple_server.out simple_server.c $(REST_LDFLAGS) $(REST_INCLUDE) $(REST_LIBDIR)

clean:
        -rm *.out
        -rm -rf *.out.dSYM
