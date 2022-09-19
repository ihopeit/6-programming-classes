#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <string.h>
#include <ulfius.h>
#include <curl/curl.h>

#include "rest_service.h"
#include "../common.h"

#define PORT 8000

#define DEFAULT_REALM  "default_realm"

/**
 * callback functions declaration
 */
int callback_city_history (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_month_ranking (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_multiple_level_final (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_default (const struct _u_request * request, struct _u_response * response, void * user_data);


int main (int argc, char **argv) {
  int ret;
  
  
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
  ulfius_add_endpoint_by_val(&instance, "GET", "pm25", "/history/*", 0, &callback_city_history, NULL);
  ulfius_add_endpoint_by_val(&instance, "GET", "pm25", "/month/*", 0, &callback_month_ranking, NULL);

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
  
  return 0;
}

int callback_city_history (const struct _u_request * request, struct _u_response * response, void * user_data){
  char * new_body;
  char * prefix = "/pm25/history/";
  CURL *curl = curl_easy_init();

  char * city_name = substr(request->http_url, strlen(prefix), strlen(request->http_url));

  if(curl) {
    int decodelen;
    char *decoded = curl_easy_unescape(curl, city_name, 0, &decodelen);
    if(decoded) {
      printf("city_name encoded:%s -> %s \n", city_name, decoded);
      new_body = msprintf("<head><meta charset=\"UTF-8\"></head>\n url: %s history data for city: %s", request->http_url, decoded);
      
      ulfius_set_string_body_response(response, 200, new_body);
      o_free(new_body);

      /* ... */
      curl_free(decoded);
    }
    curl_easy_cleanup(curl);
  }

  free(city_name);
  y_log_message(Y_LOG_LEVEL_DEBUG, "request url: %s", request->http_url);
  return U_CALLBACK_CONTINUE;
}

int callback_month_ranking (const struct _u_request * request, struct _u_response * response, void * user_data){
  char * new_body;
  char * prefix = "/pm25/month/";
  char * month = substr(request->http_url, strlen(prefix), strlen(request->http_url));
  new_body = msprintf("<head><meta charset=\"UTF-8\"></head>\n url: %s ranking by month:%s", request->http_url, month);

  ulfius_set_string_body_response(response, 200, new_body);
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