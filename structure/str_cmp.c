#include <string.h>
#include <stdio.h>
 
void comp(const char* lhs, const char* rhs){
    int rc = strcmp(lhs, rhs);
    const char *rel = rc < 0 ? "<" : rc > 0 ? ">" : "==";
    printf("[%s] %s [%s]\n", lhs, rel, rhs);
}
 
int main(void){
    comp("ted", "tee");     // ted < tee, 因为 'd' 排在 'e' 的前面
    comp("hello", "Hello"); // hello > Hello, ASCII 表中小写字母排后面
    comp("c", "abs");       // c > abs, 因为 'c' 排在 'a' 后面

    comp("Hello, everybody!" + 12, "Hello, somebody!" + 11);
    comp(&"Hello, everybody!"[12], &"Hello, somebody!"[11]);
}