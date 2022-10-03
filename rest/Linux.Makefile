all: rest_service simple_server

CFLAGS   = -ggdb -Wall

INCLUDE = -I /usr/local/include -I /usr/include/apr-1.0/
LIBDIR = -L /usr/local/lib -L /usr/lib/x86_64-linux-gnu/
LDFLAGS = -lorcania -lyder -lulfius -lcurl -lapr-1 -ljansson

rest_service: rest_service.c rest_service.h
	gcc $(CFLAGS) -o rest_service.out rest_service.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)

simple_server: simple_server.c
	gcc $(CFLAGS) -o simple_server.out simple_server.c $(LDFLAGS) $(INCLUDE) $(LIBDIR)

clean:
	-rm *.out
	-rm -rf *.out.dSYM
