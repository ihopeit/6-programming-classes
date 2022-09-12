#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <apr_general.h>
#include <apr_tables.h>
#include <apr_strings.h>

#include "pm25.h"

/* apr_array's initial size.
 * because apr_array is a dynamic array, the initial size is not a max size of the array. */
#define ARRAY_INIT_SZ           32

const char *MONTH = "2022-08-01";

typedef struct{ 
    char* city;
    char* month;
    int pm25;
} CityMonthPM25;


static void iterate_array(const apr_array_header_t *arr)
{
    int i;
    for (i = 0; i < arr->nelts; i++) {
        const CityMonthPM25 *cityPm25 = ((const CityMonthPM25**)arr->elts)[i];
        printf("%s %s pm2.5: %d \n", cityPm25 -> city, cityPm25 -> month, cityPm25 -> pm25);
    }
}

int main(int argc, char** argv){
    char *filename = "../pm25/PM25_By_Cities.csv";
    FILE* fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "Cannot open file.\n");
        exit(1);
    }


    char *line = NULL;
    size_t linecap = 0; // line capacity
    ssize_t read;

    apr_pool_t *mp;
    apr_array_header_t *arr;
        
    apr_initialize();
    apr_pool_create(&mp, NULL);

    /* Create a dynamic array of CityMonthPM25 */
    arr = apr_array_make(mp, ARRAY_INIT_SZ, sizeof(const CityMonthPM25 *));
    
    while ((read = getline(&line, &linecap, fp)) > 0){
        char **tokens;
        int count;
        count = split (line, ',', &tokens);
        
        if(count >= 7) {
            char *current_city = tokens[0];
            char *month = tokens[1];
            char *pm25 = tokens[5];
            
            CityMonthPM25 *current_city_pm25 = (CityMonthPM25 *) malloc(sizeof(CityMonthPM25 *));
            int *current_value = (int *) malloc(sizeof(int));
            *current_value = atoi(pm25);

            current_city_pm25 -> city = current_city;
            current_city_pm25 -> month = month;
            current_city_pm25 -> pm25 = *current_value;

            *(const CityMonthPM25 **)apr_array_push(arr) = current_city_pm25;
        }
    }
    
    iterate_array(arr);
    
    fclose(fp);
    if (line)
        free(line);
    return 0;
}