#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <apr_general.h>
#include <apr_tables.h>
#include <apr_strings.h>

#include "pm25.h"
#include "../common.h"

/* apr_array's initial size.
 * because apr_array is a dynamic array, the initial size is not a max size of the array. */
#define ARRAY_INIT_SZ           32

const char *MONTH = "2022-08-01";


static size_t iterate_array_for_month(const apr_array_header_t *arr, 
    const char* month, CityMonthPM25 *cities_pm25)
{
    int i;
    size_t index = 0;
    for (i = 0; i < arr->nelts; i++) {
        const CityMonthPM25 *cityPm25 = ((const CityMonthPM25**)arr->elts)[i];

        if(strcmp(cityPm25 -> month, month) == 0){
            printf("%s %s pm2.5: %d \n", cityPm25 -> city, cityPm25 -> month, cityPm25 -> pm25);
            cities_pm25[index++] = *(CityMonthPM25 *)cityPm25;
        }
    }

    return index;
}

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

    apr_pool_t *mp;
    apr_array_header_t *arr;
        
    apr_initialize();
    apr_pool_create(&mp, NULL);

    /* Create a dynamic array of CityMonthPM25 */
    arr = apr_array_make(mp, ARRAY_INIT_SZ, sizeof(const CityMonthPM25 *));

    if (argc > 1){
        target_month = argv[1];
    }

    while ((read = getline(&line, &linecap, fp)) > 0){
        char **tokens;
        int count;
        count = split (line, ',', &tokens);
        
        if(count >= 7) {
            char *current_city = tokens[0];
            char *month = tokens[1];
            char *pm25 = tokens[5];
            
            char year[5];
            memcpy( year, month, 4 );
            year[4] = '\0';
            if(!is_digit(year)){
                printf("Data with invalid month will be skipped, month: %s current_city:%s\n", month, current_city);
                continue;
            }

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
    
    CityMonthPM25 *cities_pm25 = calloc(12, sizeof(const CityMonthPM25 *));
    size_t element_count = iterate_array_for_month(arr, target_month, cities_pm25);


    qsort(cities_pm25, element_count, sizeof(CityMonthPM25), compare_month_pm25);

    printf("==== city sorted by pm2.5 at month: %s==== \n", target_month);
    while(cities_pm25 && cities_pm25 ->city ){
        printf("city: %s pm2.5: %d \n", cities_pm25 -> city, cities_pm25 -> pm25);
        cities_pm25++;
    }
    
    fclose(fp);
    if (line)
        free(line);
    return 0;
}