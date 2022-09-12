# make -f compact.makefile
# make -f compact.makefile clean
CC = gcc          ## C Compiler command
CFLAGS   = -Wall  ## C Flags, -ggdb: generate gdb debug info, -Wall: enable all warning
LDFLAGS  =        ## Library loading flags, like -lbass, -lopencv
OBJECTS  = arraylist_usage pm25_avg_sort bubble_sort quick_sort radix_sort

all: $(OBJECTS) word_counter

$(OBJECTS): %: %.c
	$(CC) $(LDFLAGS) $(CFLAGS)  -o $@ $<

word_counter: hash_demo.c hashtable.c hashtable.h
	$(CC) $(CFLAGS) -o word_counter hash_demo.c hashtable.c $(LDFLAGS)

clean:
	-rm  $(OBJECTS) word_counter