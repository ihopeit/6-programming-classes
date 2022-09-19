#include <string.h>
#include <ctype.h>

#ifndef FALSE
#define FALSE 0
#endif

// Substr for characters in `src` between `m` and `n` (excluding `n`)
// Example: char* substring = substr(line, 5, strlen(line));
char* substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char *dest = (char*)malloc(sizeof(char) * (len + 1));

    // start with m'th char and copy `len` chars into the destination
    strncpy(dest, (src + m), len);
    dest[len] = '\0';

    return dest;
}

int startsWith(const char *pre, const char *str){
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? FALSE : memcmp(pre, str, lenpre) == 0;
}

int is_digit(char* str){
    int digit_count = 0;
    for (int i = 0; i < strlen(str); i++) {
      if(isdigit(str[i]))
         digit_count++;
      else
         break;
    }

    return digit_count == strlen(str);
}
