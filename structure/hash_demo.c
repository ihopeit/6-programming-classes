// a word counter, it counts word frequencies from standard input
// from: https://github.com/benhoyt/ht/blob/master/samples/demo.c

/*
$ gcc -o word_counter.out hash_demo.c hashtable.c
$ echo 'foo bar the bar bar bar the' | ./word_counter.out

See also:
https://stackoverflow.com/questions/5134891/how-do-i-use-valgrind-to-find-memory-leaks
*/

#include "hashtable.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Example:
// $ echo 'foo bar the bar bar bar the' | ./word_counter
// foo 1
// bar 4
// the 2
// 3

void exit_nomem(void) {
    fprintf(stderr, "out of memory\n");
    exit(1);
}

void accumulate_word_count(ht* counts, char* word){
    void* value = ht_get(counts, word); // Look up word.

    // Already exists, increment int that value points to.
    if (value != NULL) {
        int* pcount = (int*)value;
        (*pcount)++;
        return;
    } 
    // Word not found, allocate space for new int and set to 1.
    int* pcount = malloc(sizeof(int));
    if (pcount == NULL) 
        exit_nomem();
    
    *pcount = 1;
    if (ht_set(counts, word, pcount) == NULL) 
        exit_nomem();
}

int main(void) {
    ht* counts = ht_create();
    if (counts == NULL)
        exit(-1);

    int year_at = 1980;
    ht_set(counts, "year", &year_at);
    ht_set(counts, "bookname", "Programming in C");
    ht_set(counts, "author", "Brian Kernighan and Dennis Ritchie");

    // Print out words and frequencies, freeing values as we go.
    int* year = (int*) ht_get(counts, "year");
    char* bookname = (char*)ht_get(counts, "bookname");
    char* author = (char*)ht_get(counts, "author");
    printf("book:%s, author:%s, year:%d\n", bookname, author, *year);

    ht_destroy(counts);
    return 0;
}

int main2(void) {
    ht* counts = ht_create();
    if (counts == NULL) {
        exit_nomem();
    }
    printf("sizeof(ht_entry): %lu\n", sizeof(ht_entry));

    // Read next word from stdin (at most 100 chars long).
    char word[101];
    while (scanf("%100s", word) != EOF) {
        accumulate_word_count(counts, word);
    }

    // Print out words and frequencies, freeing values as we go.
    hti it = ht_iterator(counts);
    while (ht_next(&it)) {
        printf("%s %d\n", it.key, *(int*)it.value);
        free(it.value);
    }

    // Show the number of unique words.
    printf("%d\n", (int)ht_length(counts));

    ht_destroy(counts);
    return 0;
}