#include <stdio.h>
#include <unistd.h>

int cfileexists(const char *filename);
void add_row_csv(int code, int value, int decimal);

int cfileexists(const char *filename)
{
    /* try to open file to read */
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void add_row_csv(int code, int value, int decimal)
{
    char mapCode[17][35] = {"Lâmpada Cozinha", "Lâmpada Sala", "Lâmpada Quarto 1", "Lâmpada Quarto 2",
    "Ar-Condicionado Quarto 1", "Ar-Condicionado Quarto 2", "Sensor de Presença Sala", "Sensor de Presença Cozinha",
    "Sensor Abertura Porta Cozinha", "Sensor Abertura Janela Cozinha", "Sensor Abertura Porta Sala", "Sensor Abertura Janela Sala",
    "Sensor Abertura Janela Quarto 1", "Sensor Abertura Janela Quarto 2", "Temperatura", "Humidade", "Temperatura Esperada"};
    char mapResponse[2][10] = {"Desligou", "Ligou"};
    char filename[10] = {"data.csv"};
    FILE *fp;
    int file_exists;
    file_exists = cfileexists(filename);
    fp = fopen(filename, "a");
    if (file_exists != 1)
    {
      fprintf(fp, "Objetivo,value");
    }
    if (code==16) {
        fprintf(fp, "\n%s, %d.%d,", mapCode[code], value, decimal);
    }
    else {
        fprintf(fp, "\n%s, %s,", mapCode[code], mapResponse[value]);
    }
    fclose(fp);
}