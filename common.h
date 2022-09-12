#include <string.h>
#include <ctype.h>

#include <apr_general.h>

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
