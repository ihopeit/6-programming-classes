/** pm25_month_sort.c
   读取 pm25 文件内容，找到指定月份各个城市的 PM2.5 并排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm25.h"

const char *MONTH = "2022-08-01";

int main(int argc, char** argv){
    char *filename = "../pm25/PM25_By_Cities.csv";
    FILE* fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "Cannot open file.\n");
        exit(1);
    }

    const char *target_month = MONTH;

    char *line = NULL;
    size_t linecap = 0; // line capacity
    ssize_t read;
    
    CityPM25 *cities_pm25 = calloc(12, sizeof(CityPM25));

    char *city = NULL;

    if(argc>1){
        target_month = argv[1];
    }
    
    int index = 0;
    while ((read = getline(&line, &linecap, fp)) > 0){
        char **tokens;
        int count, i;
        count = split (line, ',', &tokens);
        
        if(count >= 7) {
            char *current_city = tokens[0];
            char *month = tokens[1];
            char *pm25 = tokens[5];
            
            // 当前城市的月份为需要查询的月份:
            if(strcmp(month, target_month) == 0){
                city = strdup(current_city);

                int *current_value = (int *) malloc(sizeof(int));
                *current_value = atoi(pm25);
                printf("City:%s current pm2.5:%d\n", city, *current_value);

                CityPM25 *current_city_pm25 = (CityPM25 *) malloc(sizeof(CityPM25));
                current_city_pm25 -> city = city;
                current_city_pm25 -> pm25 = *current_value;
                cities_pm25[index++] = *current_city_pm25;
            }
        }

        /* freeing tokens */
        for (i = 0; i < count; i++) free (tokens[i]);
        free (tokens);
    }
    
    qsort(cities_pm25, index, sizeof(CityPM25), compare_pm25);
    printf("\nSorted PM2.5 for each city, %s:\n", target_month);
    while(cities_pm25 && cities_pm25->city){
        printf("city:%s pm2.5:%d \n", cities_pm25 ->city, cities_pm25 -> pm25);
        cities_pm25++;
    }
    
    fclose(fp);
    if (line)
        free(line);
    return 0;
}