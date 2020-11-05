#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include "central_server.h"
#include "menu.h"
#include "sockets.h"

void dataInfo(struct data *data);
void menuBorders();
void menuUser(struct data *data);

void *menu(void *params) 
// void *print_data()
{
    // double *data = params;
    struct data *data = params;

    int row,col;
    int c = 0;
    initscr ();
    curs_set (0);
    // while(1) {
    //   printf("MENU: \n");
    //   for(int i=0; i<QNT_DEVICES; i++) {
    //     printf("%d - %d\n", data[i], i);
    //   }
    //   sleep(1);
    // }

    while (1) {
        char str[80] = "";
        menuBorders();
        dataInfo(data);
        menuUser(data);
        usleep(500000);
        refresh ();
        clear();
    }
    /* End ncurses mode */
    endwin();
    return 0;
}

void dataInfo(struct data *data) {
  char mapResponse[2][10] = {"Desligado", "Ligado"};
  int row=2;

  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Informações do sistema");
  row+=5;
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Temperatura: %.2lf", data->climate->temperature);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Umidade: %.2lf", data->climate->humidity);
  row++;
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Lâmpada");
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Cozinha: %s", mapResponse[data->devices->lampKitchen]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Sala: %s", mapResponse[data->devices->lampRoom]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Quarto 01: %s", mapResponse[data->devices->lampBedroom1]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Quarto 02: %s", mapResponse[data->devices->lampBedroom2]);
  row++;
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Ar-Condicionado");
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Quarto 01: %s", mapResponse[data->devices->airConditioning1]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Quarto 02: %s", mapResponse[data->devices->airConditioning2]);
  row++;
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Sensor de presença");
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Sala: %s", mapResponse[data->sensors->spRoom]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Cozinha: %s", mapResponse[data->sensors->spKitchen]);
  row++;
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Sensor de abertura");
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Porta cozinha: %s", mapResponse[data->sensors->saDoorKitchen]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Janela cozinha: %s", mapResponse[data->sensors->saWindowKitchen]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Porta sala: %s", mapResponse[data->sensors->saDoorRoom]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Janela sala: %s", mapResponse[data->sensors->saWindowRoom]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Janela quarto 01: %s", mapResponse[data->sensors->saWindowBedroom1]);
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "  Janela quarto 02: %s", mapResponse[data->sensors->saWindowBedroom2]);
  row++;
  mvprintw ((row++)+MARGIN, MARGIN_INFO, "Temperatura esperada: %.2lf", data->expectedTemperature);

}

void menuBorders() {
    char doubleBordX[200] = {"==================================================================================================="};
    char singleBordx[200] = {"---------------------------------------------------------------------------------------------------"};
    mvprintw (0+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (4+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (35+MARGIN, 1+MARGIN, singleBordx);
    for(int row=1; row<35; row++) {
      mvprintw (row+MARGIN, 0+MARGIN, "|");
      mvprintw (row+MARGIN, 50+MARGIN, "|");
      mvprintw (row+MARGIN, 100+MARGIN, "|");
    }
}

void menuUser(struct data *data) {
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    int row = 2;
    int invertValues[2] = {1, 0}; 
    char mapResponse[2][10] = {"Ligar", "Desligar"};
    char choice[10] = {'$'};
    char controlType;
    mvprintw (row+MARGIN, 20+MARGIN, "Menu do Sistema");
    row += 5;
    mvprintw((row++)+MARGIN,2+MARGIN,"a. Definir temperatura");
    mvprintw((row++)+MARGIN,2+MARGIN,"b. Controlar Ar condicionado");
    mvprintw((row++)+MARGIN,2+MARGIN,"c. Controlar lâmpada");
    mvprintw((row++)+MARGIN,2+MARGIN,"Pressione CTRL + C para sair do programa");
    row++;
    mvprintw((row++)+MARGIN,2+MARGIN,"Digite a sua escolha:  ");
    row++;

    if( poll(&mypoll, 1, 0) ){
      getstr(choice);
    }

    if(!strcmp(choice, "a")) {
      mvprintw((row++)+MARGIN,2+MARGIN,"Digite a temperatura desejada: ");
      scanw(" %4lf", &data->expectedTemperature) ;
      int integer = (int)data->expectedTemperature;
      int decimal = (int)((data->expectedTemperature-integer)*10);
      send_data(TEMPERATURE_CONTROL, integer, decimal);
    }
    else if(!strcmp(choice, "b")) {
      mvprintw((row++)+MARGIN,2+MARGIN,"1 - %s ar condicionado 1", mapResponse[data->devices->airConditioning1]);
      mvprintw((row++)+MARGIN,2+MARGIN,"2 - %s ar condicionado 2", mapResponse[data->devices->airConditioning2]);
      row++;
      int conditionalAir;
      mvprintw((row++)+MARGIN,2+MARGIN,"Digite o número do ar condicionado: ");
      scanw(" %d", &conditionalAir);
      // mvprintw((row++)+MARGIN,2+MARGIN,"Valor escolhido = %d, valor atual = %d", conditionalAir, data->devices->airConditioning1);
      if (conditionalAir == 1) {
        send_data(AIR_BEDROOM_1, invertValues[data->devices->airConditioning1], 0);
      }
      else if(conditionalAir == 2) {
        send_data(AIR_BEDROOM_2, invertValues[data->devices->airConditioning2], 0);
      }
      else {
        mvprintw((row++)+MARGIN,2+MARGIN,"Ativação cancelada");
      }
    }
    else if(!strcmp(choice, "c")) {
      mvprintw((row++)+MARGIN,2+MARGIN,"1 - %s Lâmpada Cozinha", mapResponse[data->devices->lampKitchen]);
      mvprintw((row++)+MARGIN,2+MARGIN,"2 - %s Lâmpada Sala", mapResponse[data->devices->lampRoom]);
      mvprintw((row++)+MARGIN,2+MARGIN,"3 - %s Lâmpada Quarto 1", mapResponse[data->devices->lampBedroom1]);
      mvprintw((row++)+MARGIN,2+MARGIN,"4 - %s Lâmpada Quarto 2", mapResponse[data->devices->lampBedroom2]);
      row++;
      int lamp;
      mvprintw((row++)+MARGIN,2+MARGIN,"Digite o número da lâmpada: ");
      scanw(" %d", &lamp);
      if (lamp == 1) {
        send_data(L_KITCHEN, invertValues[data->devices->lampKitchen], 0);
      }
      else if(lamp == 2) {
        send_data(L_ROOM, invertValues[data->devices->lampRoom], 0);
      }
      else if(lamp == 3) {
        send_data(L_BEDROOM_1, invertValues[data->devices->lampBedroom1], 0);
      }
      else if(lamp == 4) {
        send_data(L_BEDROOM_2, invertValues[data->devices->lampBedroom2], 0);
      }
      else {
        mvprintw((row++)+MARGIN,2+MARGIN,"Ativação cancelada");
      }
    }
    else if(!strcmp(choice, "$")) {
      mvprintw((row++)+MARGIN,2+MARGIN,"");
    }
    else {
      mvprintw((row++)+MARGIN,2+MARGIN,"Escolha inválida");
    }
}
