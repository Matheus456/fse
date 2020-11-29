typedef struct Infomarcoes
{
    double temperatura;
    double temperatura_max;
    double temperatura_min;
    double humidade;
    double latitude;
    double longitude;
} Infomarcoes;
void get_json_info(char * buffer);