#include <stdio.h>
#include <stdlib.h>
#define Datatype int

struct tag_name { 
    Datatype member_a;
    Datatype member_b;
    //...
} variable_1, variable_2, variable_3 ;

/* 定义两个匿名结构体的变量 */
struct {
     int x;
     int y;
} point1, point2; 

int main(int argc, char* argv[]){
    point1.x = 5;
    point2.x = 10;
    printf("x1 + x2 = %d \n", point1.x + point2.x );
}

 typedef struct {
     int x;
     int y;
 } Point; //结构体别名为 Point

void test(){
    // 定义点:
    Point p1; 
    Point p2;
    Point *po;

    /* 点操作符:结构体变量访问其成员 */
    p1.x = 10;
    p2.y = 20;
    p1.y = p2.y;

    /* 箭头操作符：结构体指针访问成员变量 */
    int h1 = po->x;
    int h2 = (*po).x;
}
