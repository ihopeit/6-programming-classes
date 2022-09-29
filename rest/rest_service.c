#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <string.h>
#include <ulfius.h>
#include <curl/curl.h>

#include <apr_tables.h>
#include <apr_strings.h>

#include "rest_service.h"
#include "../common.h"
#include "../pm25/pm25.h"

/* apr_array's initial size.
 * because apr_array is a dynamic array, the initial size is not a max size of the array. */
#define ARRAY_INIT_SZ           32

const char *MONTH = "2022-08"; 

static size_t iterate_array_for_city(const apr_array_header_t *arr, 
    const char* city, CityMonthPM25 *cities_pm25)
{
    int i;
    size_t index = 0;
    for (i = 0; i < arr->nelts; i++) {
        const CityMonthPM25 *cityPm25 = ((const CityMonthPM25**)arr->elts)[i];

        if(strcmp(cityPm25 ->city, city) == 0){
            printf("iterated %s %s pm2.5: %d \n", cityPm25 -> city, cityPm25 -> month, cityPm25 -> pm25);
            cities_pm25[index++] = *(CityMonthPM25 *)cityPm25;
        }
    }

    return index;
}

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
        fprintf(stderr, "Cannot open file. %s\n", csv_data_file_name);
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

            //CityMonthPM25 *current_city_pm25 = (CityMonthPM25 *) malloc(sizeof(CityMonthPM25));
            CityMonthPM25 *current_city_pm25 = (CityMonthPM25 *) malloc(sizeof(*current_city_pm25));

            current_city_pm25 -> city = strdup(current_city);
            current_city_pm25 -> month = strdup(month);
            current_city_pm25 -> pm25 = atoi(pm25);
            // push an element to the dynamic array:
            *(const CityMonthPM25 **)apr_array_push(arr) = current_city_pm25;
        }

        /* freeing tokens */
        for (int i = 0; i < count; i++) 
            free (tokens[i]);
        free (tokens);
    }
    
    fclose(fp);
    if (line)
        free(line);
}


#define PORT 8000

#define DEFAULT_REALM  "default_realm"

/**
 * callback functions declaration
 */
int callback_city_history (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_month_ranking (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_multiple_level_final (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_default (const struct _u_request * request, struct _u_response * response, void * user_data);

CURL *curl;

int main (int argc, char **argv) {
  int ret;

  char *filename = "../pm25/PM25_By_Cities_Month.csv";

  if (argc > 1){
    filename = argv[1];
  }

  apr_pool_t *mp;
  apr_array_header_t *arr;
      
  apr_initialize();
  apr_pool_create(&mp, NULL);

  curl = curl_easy_init();

  /* Create a dynamic array of CityMonthPM25 */
  arr = apr_array_make(mp, ARRAY_INIT_SZ, sizeof(const CityMonthPM25 *));

  init_city_data(filename, arr);
  
  // Set the framework port number
  struct _u_instance instance;
  
  y_init_logs("pm25_rest_service", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "Starting pm25 service");
  
  if (ulfius_init_instance(&instance, PORT, NULL, DEFAULT_REALM) != U_OK) {
    y_log_message(Y_LOG_LEVEL_ERROR, "Error ulfius_init_instance, abort");
    return(1);
  }
  
  u_map_put(instance.default_headers, "Access-Control-Allow-Origin", "*");
  
  // Maximum body size sent by the client is 1 Kb
  instance.max_post_body_size = 1024;

  // Endpoint list declaration
  ulfius_add_endpoint_by_val(&instance, "GET", "pm25", "/history/*", 0, &callback_city_history, arr);
  ulfius_add_endpoint_by_val(&instance, "GET", "pm25", "/month/*", 0, &callback_month_ranking, arr);

  // callback final to release memories assigned:
  ulfius_add_endpoint_by_val(&instance, "*", "pm25", "/*", 99, &callback_multiple_level_final, NULL);
  
  // default_endpoint declaration, return 404 for other URLs
  ulfius_set_default_endpoint(&instance, &callback_default, NULL);
  
  // Start the framework
  ret = ulfius_start_framework(&instance);
  
  if (ret == U_OK) {
    y_log_message(Y_LOG_LEVEL_DEBUG, "Start framework on port %d", instance.port);
    
    // Wait for the user to press <enter> on the console to quit the application
    getchar();
  } else {
    y_log_message(Y_LOG_LEVEL_DEBUG, "Error starting framework");
  }
  y_log_message(Y_LOG_LEVEL_DEBUG, "End framework");
  
  y_close_logs();
  
  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  if(curl){
    curl_easy_cleanup(curl);
  }
  
  return 0;
}

int callback_city_history (const struct _u_request * request, struct _u_response * response, void * user_data){
  char * new_body;
  char * prefix = "/pm25/history/";

  char * city_name = substr(request->http_url, strlen(prefix), strlen(request->http_url));

  if(curl) {
    int decodelen;
    char *decoded_city = curl_easy_unescape(curl, city_name, 0, &decodelen);
    if(decoded_city) {
      printf("city_name encoded:%s -> %s \n", city_name, decoded_city);
      asprintf(&new_body, "<head><meta charset=\"UTF-8\"></head>\n url: %s history data for city: %s <br>", request->http_url, decoded_city);
      
      apr_array_header_t *arr = user_data;

      //CityMonthPM25 *cities_pm25   = calloc(120, sizeof(*cities_pm25));
      CityMonthPM25 *cities_pm25   = calloc(120, sizeof(CityMonthPM25));

      CityMonthPM25 *cities_pm25_ptr = cities_pm25;
      size_t element_count = iterate_array_for_city(arr, decoded_city, cities_pm25);
      

      printf("==== historical city pm2.5: %s count:%ld==== <br><br>\n", decoded_city, element_count);
      // 分配到 cities_pm25 的空间数量大于实际存储的数据量 (2013年1月到2022年9月，<10年数据, 不到120个月)
      // 处理 elementCount 条数据
      json_t *array = json_array();
      while(element_count>0 && cities_pm25 && cities_pm25->month && cities_pm25->pm25)
      {
        // char * old_body = new_body;
        // printf("month: %s pm2.5: %d <br>\n", cities_pm25->month, cities_pm25->pm25);
        // asprintf(&new_body,"%s month: %s pm2.5: %d <br>\n", new_body, cities_pm25->month, cities_pm25->pm25);

        json_t * json_body = json_object();
        json_object_set_new(json_body, "month", json_string(cities_pm25->month));
        json_object_set_new(json_body, "pm2.5", json_integer(cities_pm25->pm25));
        json_array_append_new(array, json_body);

        // free(old_body);

        cities_pm25++;
        element_count--;
      }

      // ulfius_set_string_body_response(response, 200, new_body);
      ulfius_set_json_body_response(response, 200, array);
      json_decref(array);

      free(new_body);
      new_body = NULL;

      // memory should be freed here:
      free(cities_pm25_ptr);

      /* ... */
      curl_free(decoded_city);
      decoded_city = NULL;
    }

  }

  free(city_name);
  y_log_message(Y_LOG_LEVEL_DEBUG, "request url: %s", request->http_url);
  return U_CALLBACK_CONTINUE;
}

int callback_month_ranking (const struct _u_request * request, struct _u_response * response, void * user_data){
  char * new_body;
  char * prefix = "/pm25/month/";
  char * target_month = substr(request->http_url, strlen(prefix), strlen(request->http_url));
  new_body = msprintf("<head><meta charset=\"UTF-8\"></head>\n url: %s ranking by month:%s <br>", request->http_url, target_month);

  apr_array_header_t *arr = user_data;
  CityMonthPM25 *cities_pm25 = calloc(12, sizeof(CityMonthPM25));
  // CityMonthPM25 *cities_pm25 = calloc(12, sizeof(*cities_pm25));

  CityMonthPM25 *head = cities_pm25;
  size_t element_count = iterate_array_for_month(arr, target_month, cities_pm25);

  qsort(cities_pm25, element_count, sizeof(CityMonthPM25), compare_month_pm25);

  printf("==== city sorted by pm2.5 at month: %s==== <br>\n", target_month);
  json_t *array = json_array();
  while (cities_pm25 && cities_pm25->city)
  {
    // char * old_body = new_body;
    // new_body = msprintf("%s \n city: %s pm2.5: %d <br>\n <br>", old_body, cities_pm25->city, cities_pm25->pm25);
    json_t * json_body = json_object();
    json_object_set_new(json_body, "city", json_string(cities_pm25->city));
    json_object_set_new(json_body, "pm2.5", json_integer(cities_pm25->pm25));
    json_array_append_new(array, json_body);

    printf("city: %s pm2.5: %d <br>\n", cities_pm25->city, cities_pm25->pm25);
    cities_pm25++;
    // o_free(old_body);
  }

  free(head);
  free(target_month);

  //ulfius_set_string_body_response(response, 200, new_body); // response plain html
  ulfius_set_json_body_response(response, 200, array);
  json_decref(array);

  o_free(new_body);
  y_log_message(Y_LOG_LEVEL_DEBUG, "request url: %s", response->shared_data);
  return U_CALLBACK_CONTINUE;
}


/**
 * Final callback function called on multiple calls
 */
int callback_multiple_level_final (const struct _u_request * request, struct _u_response * response, void * user_data) {
  o_free(response->shared_data);
  return U_CALLBACK_COMPLETE;
}


/**
 * Default callback function called if no endpoint has a match
 */
int callback_default (const struct _u_request * request, struct _u_response * response, void * user_data) {
  ulfius_set_string_body_response(response, 404, "Page not found, do what you want");
  return U_CALLBACK_CONTINUE;
}