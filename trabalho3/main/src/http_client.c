#include <string.h>

#include "esp_log.h"
#include "esp_event.h"
#include "esp_http_client.h"

#include "json_parser.h"
#include "http_client.h"

#define TAG "HTTP"



extern Infomarcoes informacoes;


esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    static char *buffer;
    static int output = 0; 
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char*)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) 
            {
                if (evt->user_data) {
                    memcpy(evt->user_data + output, evt->data, evt->data_len);
                }
                else 
                {
                    if (buffer == NULL) {
                        buffer = (char *) malloc(esp_http_client_get_content_length(evt->client)+1);
                        output = 0;
                        if (buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                        buffer[esp_http_client_get_content_length(evt->client)] = '\0';
                    }
                    memcpy(buffer + output, evt->data, evt->data_len);
                }
                output += evt->data_len;                
            }
            else
            {
                if (buffer == NULL) {
                    buffer = (char *) malloc(evt->data_len+1);
                    if (buffer == NULL) {
                        ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                        return ESP_FAIL;
                    }
                    buffer[evt->data_len] = '\0';
                }
                memcpy(buffer + output, evt->data, evt->data_len);
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            if (buffer != NULL) {
                get_json_info(buffer);
                free(buffer);
                buffer = NULL;
            }
            output = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

void http_request(char *url){
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handle,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
    ESP_LOGI(TAG, "Status = %d, content_length = %d",
            esp_http_client_get_status_code(client),
            esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);
}

void https_request()
{
    esp_http_client_config_t config = {
        .url = "https://covid19-brazil-api.now.sh/api/report/v1",
        .event_handler = _http_event_handle,
        //.cert_pem = howsmyssl_com_root_cert_pem_start,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);
}
