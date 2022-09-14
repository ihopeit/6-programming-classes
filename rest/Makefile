all: rest_service

CFLAGS   = -ggdb -Wall

REST_INCLUDE = -I /opt/homebrew/Cellar/gnutls/3.7.7/include \
    -I /opt/homebrew/Cellar/jansson/2.14/include \
	-I /opt/homebrew/Cellar/libmicrohttpd/0.9.75/include -I /usr/local/include
REST_LIBDIR = -L /opt/homebrew/Cellar/gnutls/3.7.7/lib -L /usr/local/lib
REST_LDFLAGS = -lorcania -lyder -lulfius

rest_service: rest_service.c rest_service.h
	gcc $(CFLAGS) -o rest_service.out rest_service.c $(REST_LDFLAGS) $(REST_INCLUDE) $(REST_LIBDIR)

clean:
	-rm *.out
	-rm -rf *.out.dSYM
