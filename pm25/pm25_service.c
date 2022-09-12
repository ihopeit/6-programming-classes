#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <apr_tables.h>
#include <apr_strings.h>

#include "pm25.h"
#include "../common.h"

/* apr_array's initial size.
 * because apr_array is a dynamic array, the initial size is not a max size of the array. */
#define ARRAY_INIT_SZ           32

const char *MONTH = "2022-08"; 

static size_t iterate_array_for_month(const apr_array_header_t *arr, 
    const char* month, CityMonthPM25 *cities_pm25)
{
    int i;
    size_t index = 0;
    for (i = 0; i < arr->nelts; i++) {
        const CityMonthPM25 *cityPm25 = ((const CityMonthPM25**)arr->elts)[i];

        if(strcmp(cityPm25 -> month, month) == 0){
            //printf("%s %s pm2.5: %d \n", cityPm25 -> city, cityPm25 -> month, cityPm25 -> pm25);
            cities_pm25[index++] = *(CityMonthPM25 *)cityPm25;
        }
    }

    return index;
}

/* 月份字符串是否为有效的格式，如 2021-05 表示 2021年5月 */
int is_valid_month(const char *month){
    char year[5];
    memcpy(year, month, 4);
    year[4] = '\0';

    return is_digit(year);
}

// read pm2.5 data for each city of each month from file
void init_city_data(char* csv_data_file_name, apr_array_header_t *arr){
    FILE* fp = fopen(csv_data_file_name, "r");
    if(!fp){
        fprintf(stderr, "Cannot open file.\n");
        exit(1);
    }

    char *line = NULL;
    size_t linecap = 0; // line capacity
    ssize_t read;
    while ((read = getline(&line, &linecap, fp)) > 0){
        char **tokens;
        int count = split (line, ',', &tokens);

        if(count >= 7 && is_valid_month(tokens[1])) { // 忽略第一行表头的无效月份
            char *current_city = tokens[0];
            char *month = tokens[1];
            char *pm25 = tokens[5];

            CityMonthPM25 *current_city_pm25 = (CityMonthPM25 *) malloc(sizeof(CityMonthPM25 *));
            int *current_value = (int *) malloc(sizeof(int));
            *current_value = atoi(pm25);

            current_city_pm25 -> city = current_city;
            current_city_pm25 -> month = month;
            current_city_pm25 -> pm25 = *current_value;
            // push an element to the dynamic array:
            *(const CityMonthPM25 **)apr_array_push(arr) = current_city_pm25;
        }
    }
    
    fclose(fp);
    if (line)
        free(line);
}

int main(int argc, char** argv){
    char *filename = "PM25_By_Cities_Month.csv";

    const char *target_month = MONTH;
    apr_pool_t *mp;
    apr_array_header_t *arr;
        
    apr_initialize();
    apr_pool_create(&mp, NULL);

    /* Create a dynamic array of CityMonthPM25 */
    arr = apr_array_make(mp, ARRAY_INIT_SZ, sizeof(const CityMonthPM25 *));

    if (argc > 1){
        target_month = argv[1];
    }

    init_city_data(filename, arr);

    // get QUERY_STRING for CGI Request:
    char* query_string = getenv("QUERY_STRING");

    if(query_string && startsWith("month=", query_string)){
        // 从索引6 开始拷贝7个字符，结果如 2022-05
        char month_from_cgi_query[8];
        memcpy(month_from_cgi_query, &query_string[6], 7);
        month_from_cgi_query[7] = '\0';

        target_month = month_from_cgi_query;

        // for CGI Request, print header
        printf("Content-type: text/html\r\n"
        "\r\n"

        "<title>PM2.5 for cities sorted!</title>\n"
        "<h1>PM2.5 list for month:%s <br>\n</h1>"

        "Service running on host <i>%s</i> <br>\n\n",

        target_month, getenv("SERVER_NAME"));
    }

    CityMonthPM25 *cities_pm25 = calloc(12, sizeof(const CityMonthPM25 *));
    size_t element_count = iterate_array_for_month(arr, target_month, cities_pm25);

    qsort(cities_pm25, element_count, sizeof(CityMonthPM25), compare_month_pm25);

    printf("==== city sorted by pm2.5 at month: %s==== <br>\n", target_month);
    while(cities_pm25 && cities_pm25 ->city ){
        printf("city: %s pm2.5: %d <br>\n", cities_pm25 -> city, cities_pm25 -> pm25);
        cities_pm25++;
    }

    return 0;
}
