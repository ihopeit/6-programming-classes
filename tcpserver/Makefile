all: server client simple_cache_server

server: server.c
	gcc -o server.out server.c
client: client.c
	gcc -o client.out client.c
simple_cache_server: simple_cache_server.c
	gcc -o simple_cache_server.out simple_cache_server.c ../structure/hashtable.c
clean:
	rm *.out
