/** pm25_avg_sort.c
   读取 pm25 文件内容，统计每个城市 2022-01 ~ 2022-05 的平均 PM2.5 并排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *MIN_MONTH = "2022-01";
const char *MAX_MONTH = "2022-05";

//使用结构体来存储一个城市的 PM25 值
typedef struct{ 
    char* city;
    int pm25;
} CityPM25;

int split (const char *txt, char delim, char ***tokens){
    int *tklen, *t, count = 1;
    char **arr, *p = (char *) txt;

    while (*p != '\0') if (*p++ == delim) count += 1;
    t = tklen = calloc (count, sizeof (int));
    for (p = (char *) txt; *p != '\0'; p++) *p == delim ? *t++ : (*t)++;
    *tokens = arr = malloc (count * sizeof (char *));
    t = tklen;
    p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
    while (*txt != '\0'){
        if (*txt == delim){
            p = *arr++ = calloc (*(t++) + 1, sizeof (char *));
            txt++;
        }
        else *p++ = *txt++;
    }
    free (tklen);
    return count;
}

int compare_pm25(const void* arg1, const void* arg2){ 
    const CityPM25 *a = (const CityPM25*) arg1;
    const CityPM25 *b = (const CityPM25*) arg2;

    if (a->pm25 < b->pm25) return -1;
    if (a->pm25 > b->pm25) return 1;
    return 0;
}


int main(int argc, char** argv){
    char *filename = "../pm25/PM25_By_Cities_Month.csv";
    FILE* fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "Cannot open file.\n");
        exit(1);
    }

    char *line = NULL;
    size_t linecap = 0; // line capacity
    ssize_t read;
    
    CityPM25 *cities_pm25 = calloc(12, sizeof(CityPM25));

    char *city = NULL;
    int data_count = 0;
    long total_pm25 = 0L;
    int index = 0;
    while ((read = getline(&line, &linecap, fp)) > 0){
        char **tokens;
        int count, i;
        count = split (line, ',', &tokens);
        
        if(count >= 7) {
            char *current_city = tokens[0];
            char *month = tokens[1];
            char *pm25 = tokens[5];

            if(city==NULL) city = strdup(current_city);
            
            // 同一个城市，且月份在最小月份（1月）到最大月份（5月）之间:
            if(strcmp(current_city, city)==0 && 
               strcmp(month, MIN_MONTH) >= 0 &&
               strcmp(month, MAX_MONTH) <=0 ){
                data_count++;
                total_pm25 += atoi(pm25);

                if(strcmp(month, MAX_MONTH) == 0){
                    int *average = (int *) malloc(sizeof(int));
                    *average = total_pm25/data_count;
                    printf("City:%s average pm2.5:%d\n", city, *average);

                    CityPM25 *current_city_pm25 = (CityPM25 *) malloc(sizeof(CityPM25));
                    current_city_pm25 -> city = city;
                    current_city_pm25 -> pm25 = *average;
                    cities_pm25[index++] = *current_city_pm25;
                }
            }else {
                //重新开始统计新城市的数据
                city = strdup(current_city);
                data_count = 0;
                total_pm25 = 0L;
            }
            //printf ("City:%s month:%s pm2.5:%s\n", city, month, pm25);
        }

        /* freeing tokens */
        for (i = 0; i < count; i++) free (tokens[i]);
        free (tokens);
    }
    
    qsort(cities_pm25, index, sizeof(CityPM25), compare_pm25);
    printf("\nSorted PM2.5 for each city, %s - %s:\n", MIN_MONTH, MAX_MONTH);
    while(cities_pm25 && cities_pm25->city){
        printf("city:%s pm2.5:%d \n", cities_pm25 ->city, cities_pm25 -> pm25);
        cities_pm25++;
    }
    
    fclose(fp);
    if (line)
        free(line);
    return 0;
}