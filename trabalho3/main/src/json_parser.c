#include <stdio.h>
#include "json_parser.h"
#include "cJSON.h"

extern Infomarcoes informacoes;

void get_json_info(char * buffer)
{
    cJSON * json = cJSON_Parse (buffer);
    // Ip Stack
    if (buffer[2] == 'l') //latitude
    {
        informacoes.longitude = cJSON_GetObjectItemCaseSensitive(json, "longitude")->valuedouble;
        informacoes.latitude = cJSON_GetObjectItemCaseSensitive(json, "latitude")->valuedouble;
    }
    // Open Weather Map
    else if (buffer[2] == 'c') //coord
    {
        cJSON * obj_json = cJSON_GetObjectItem(json, "main");
        informacoes.temperatura = cJSON_GetObjectItemCaseSensitive(obj_json, "temp")->valuedouble;
        informacoes.temperatura_min = cJSON_GetObjectItemCaseSensitive(obj_json, "temp_min")->valuedouble;
        informacoes.temperatura_max = cJSON_GetObjectItemCaseSensitive(obj_json, "temp_max")->valuedouble;
        informacoes.humidade = cJSON_GetObjectItemCaseSensitive(obj_json, "humidity")->valuedouble;
    }
    cJSON_Delete(json);
}