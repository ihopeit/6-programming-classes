all: rest_service simple_server

CFLAGS   = -ggdb -Wall

REST_INCLUDE = -I /opt/homebrew/Cellar/gnutls/3.7.7/include \
    -I /opt/homebrew/Cellar/jansson/2.14/include \
	-I /opt/homebrew/Cellar/libmicrohttpd/0.9.75/include -I /usr/local/include \
	-I /opt/homebrew/Cellar/apr/1.7.0_3/include/apr-1 
REST_LIBDIR = -L /opt/homebrew/Cellar/gnutls/3.7.7/lib -L /usr/local/lib \
	-L /opt/homebrew/Cellar/apr/1.7.0_3/lib  -L /opt/homebrew/Cellar/jansson/2.14/lib/
REST_LDFLAGS = -lorcania -lyder -lulfius -lcurl -lapr-1 -ljansson

rest_service: rest_service.c rest_service.h
	gcc $(CFLAGS) -o rest_service.out rest_service.c $(REST_LDFLAGS) $(REST_INCLUDE) $(REST_LIBDIR)

simple_server: simple_server.c
	gcc $(CFLAGS) -o simple_server.out simple_server.c $(REST_LDFLAGS) $(REST_INCLUDE) $(REST_LIBDIR)

clean:
	-rm *.out
	-rm -rf *.out.dSYM
