#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // 定义一个成绩单
 typedef struct {
     float chinese;
     float math;
     float english;
     float pe;
 } ReportCard;

// 某个学生的成绩单
 typedef struct{
    char student_name[50];
    ReportCard report;
 } StudentReport;

struct Book
{
   char  title[50];
   char  author[50];
   char  subject[100];
   int   book_id;
} book;

int main(int argc, char* argv[]){
   struct Book book2;
   strcpy(book.title, "Python深度学习");
   strcpy(book.author, "李涛");
   strcpy(book2.title, "The C Programming Language");
   printf("title:%s author:%s, \nbook2:%s \n", book.title, book.author, book2.title);
   return 0;
}
