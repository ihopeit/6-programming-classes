#include <string.h>
#include <ctype.h>

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
