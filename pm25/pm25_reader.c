/** pm25_reader.c
   读取 pm25 文件内容，按行输出内容
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){
    char *filename = "PM25_By_Cities.csv";
    FILE* fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "Cannot open file.\n");
        exit(1);
    }

    char *line = NULL;
    size_t linecap = 0; // line capacity
    ssize_t read;
    
    while ((read = getline(&line, &linecap, fp)) > 0)
        fwrite(line, read, 1, stdout);
    
    fclose(fp);
    if (line)
        free(line);
    return 0;
}