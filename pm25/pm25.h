#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//使用结构体来存储一个城市的 PM25 值
typedef struct{ 
    char* city;
    int pm25;
} CityPM25;


typedef struct{ 
    char* city;
    char* month;
    int pm25;
} CityMonthPM25;


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

int compare_month_pm25(const void* arg1, const void* arg2){ 
    const CityMonthPM25 *a = (const CityMonthPM25*) arg1;
    const CityMonthPM25 *b = (const CityMonthPM25*) arg2;

    if (a->pm25 < b->pm25) return -1;
    if (a->pm25 > b->pm25) return 1;
    return 0;
}