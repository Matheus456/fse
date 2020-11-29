#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "http_client.h"
#include "led_controll.h"
#include "json_parser.h"

#define IP_TACK_KEY CONFIG_IPSTACK_ACCESS_KEY
#define OPEN_WEATHER_MAP_KEY CONFIG_OPENWEATHERMAP_ACCESS_KEY

#define DELAY 300000 // 5min


xSemaphoreHandle conexaoLedSemaphore = NULL;
xSemaphoreHandle conexaoWifiSemaphore = NULL;

Infomarcoes informacoes;


void PiscaLed(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(conexaoLedSemaphore, portMAX_DELAY))
        {
            pisca_led();
            xSemaphoreGive(conexaoLedSemaphore);
        }
    }
}

void RequestIpStack(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
        {
            pisca_led();
            printf("Requisição IP STACK");
            ESP_LOGI("IP Stack request", "Requisição IP Stack");
            char url[110];
            sprintf(url,
                    "http://api.ipstack.com/check?access_key=%s&fields=latitude,longitude",
                    IP_TACK_KEY);
            http_request(url);
            xSemaphoreGive(conexaoWifiSemaphore);
            vTaskDelay(DELAY / portTICK_PERIOD_MS);
        }
    }
}


void RequestOpenWeatherMap(void *params)
{
    while (true)
    {
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
        {
            pisca_led();
            printf("Requisição Open Weather Map");
            ESP_LOGI("IP Open Weather Map", "Requisição Open Weather Map");
            char url[150];
            sprintf(url,
                    "http://api.openweathermap.org/data/2.5/weather?lat=%lf&lon=%lf&units=metric&appid=%s",
                    informacoes.latitude,
                    informacoes.longitude,
                    OPEN_WEATHER_MAP_KEY);
            http_request(url);
            printf("\nTemperatura Mínima: %.1lf°C\n", informacoes.temperatura_min);
            printf("Temperatura Máxima: %.1lf°C\n", informacoes.temperatura_max);
            printf("Temperatura Atual: %.1lf°C\n", informacoes.temperatura);
            printf("Humidade: %.1lf%%\n", informacoes.humidade);
            xSemaphoreGive(conexaoWifiSemaphore);
            vTaskDelay(DELAY / portTICK_PERIOD_MS);
        }
        
    }
}


void app_main(void)
{
    conexaoLedSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(conexaoLedSemaphore);
    config_led();
    xTaskCreate(&PiscaLed, "Blink", 1024, NULL, 1, NULL);
    
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&RequestIpStack, "HTTP Request Location", 4096, NULL, 1, NULL);
    xTaskCreate(&RequestOpenWeatherMap, "HTTP Request Weather", 4096, NULL, 1, NULL);
}