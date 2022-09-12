CC = gcc          ## C Compiler command
CFLAGS   = -Wall  ## C Flags, -ggdb: generate gdb debug info, -Wall: enable all warning
LDFLAGS  =        ## Library loading flags, like -lbass, -lopencv

all: arraylist_usage pm25_avg_sort word_counter bubble_sort quick_sort radix_sort

arraylist_usage: arraylist_usage.c
	$(CC) $(CFLAGS) -o arraylist_usage.out arraylist_usage.c  $(LDFLAGS)

pm25_avg_sort: pm25_avg_sort.c
	$(CC) $(CFLAGS) -o pm25_avg_sort.out pm25_avg_sort.c $(LDFLAGS)

word_counter: hash_demo.c hashtable.c hashtable.h
	$(CC) $(CFLAGS) -o word_counter.out hash_demo.c hashtable.c $(LDFLAGS)

bubble_sort: bubble_sort.c
	$(CC) $(CFLAGS) -o bubble_sort.out bubble_sort.c $(LDFLAGS)

quick_sort: quick_sort.c
	$(CC) $(CFLAGS) -o quick_sort.out quick_sort.c $(LDFLAGS)

radix_sort: radix_sort.c
	$(CC) $(CFLAGS) -o radix_sort.out radix_sort.c $(LDFLAGS)

clean:
	rm *.out
