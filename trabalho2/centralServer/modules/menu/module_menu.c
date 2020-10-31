#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include "../../central_server.h"
#include "module_menu.h"

void dataInfo(struct data *data);
void menuBorders();
// void menuUser(double *data);

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
        // menuUser(data);
        usleep(500000);
        refresh ();
        clear();
    }
    /* End ncurses mode */
    endwin();
    return 0;
}

void dataInfo(struct data *data) {
  int i=2;

  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Informações do sistema");
  i+=5;
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Temperatura: %.2lf", data->climate->temperature);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Umidade: %.2lf", data->climate->humidity);
  i++;
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Lâmpada");
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Cozinha: %d", data->devices->lampKitchen);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Sala: %d", data->devices->lampRoom);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Quarto 01: %d", data->devices->lampBedroom1);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Quarto 02: %d", data->devices->lampBedroom2);
  i++;
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Ar-Condicionado");
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Quarto 01: %d", data->devices->airConditioning1);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Quarto 02: %d", data->devices->airConditioning2);
  i++;
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Sensor de presença");
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Sala: %d", data->sensors->spRoom);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Cozinha: %d", data->sensors->spKitchen);
  i++;
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "Sensor de abertura");
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Porta cozinha: %d", data->sensors->saDoorKitchen);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Janela cozinha: %d", data->sensors->saWindowKitchen);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Porta sala: %d", data->sensors->saDoorRoom);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Janela sala: %d", data->sensors->saWindowRoom);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Janela quarto 01: %d", data->sensors->saWindowBedroom1);
  mvprintw ((i++)+MARGIN, MARGIN_INFO, "  Janela quarto 02: %d", data->sensors->saWindowBedroom2);
}

void menuBorders() {
    char doubleBordX[200] = {"==================================================================================================="};
    char singleBordx[200] = {"---------------------------------------------------------------------------------------------------"};
    mvprintw (0+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (4+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (35+MARGIN, 1+MARGIN, singleBordx);
    for(int i=1; i<35; i++) {
      mvprintw (i+MARGIN, 0+MARGIN, "|");
      mvprintw (i+MARGIN, 50+MARGIN, "|");
      mvprintw (i+MARGIN, 100+MARGIN, "|");
    }
}

// void menuUser(double *data) {
//     struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
//     char choice[10] = {'$'};
//     char controlType;
//     mvprintw (2+MARGIN, 20+MARGIN, "Menu do Sistema");
//     mvprintw(5+MARGIN,2+MARGIN,"a. Alterar temperatura");
//     mvprintw(6+MARGIN,2+MARGIN,"b. Alterar histerese");
//     mvprintw(7+MARGIN,2+MARGIN,"c. Alterar tipo de controle");
//     mvprintw(8+MARGIN,2+MARGIN,"Pressione CTRL + C para sair do programa");

//     mvprintw(9+MARGIN,2+MARGIN,"Digite a sua escolha:");
//     if( poll(&mypoll, 1, 0) ){
//       getstr(choice);
//       mvprintw(20+MARGIN,2+MARGIN, "Choice |%s|", choice);
//     }

//     if(!strcmp(choice, "a")) {
//       mvprintw(11+MARGIN,2+MARGIN,"Digite a temperatura: ");
//       scanw(" %4lf", &data->tr) ;
//       strcpy(data->inputType, "t");
//     }
//     else if(!strcmp(choice, "b")) {
//       mvprintw(11+MARGIN,2+MARGIN,"Digite o valor da histerese: ");
//       scanw(" %4lf", &data->hysteresis) ;
//     }
//     else if(!strcmp(choice, "c")) {
//       mvprintw(11+MARGIN,2+MARGIN,"Digite o controle desejado");
//       mvprintw(12+MARGIN,2+MARGIN,"(p -> Potenciômetro/ t -> teclado):");
//       char controle[10];
//       getstr(controle);
//       if (!strcmp(controle, "p") || !strcmp(controle, "t")) {
//         strcpy(data->inputType, controle);
//         mvprintw(13+MARGIN,2+MARGIN,"Controle alterado");
//       }
//       else {
//         mvprintw(13+MARGIN,2+MARGIN,"Escolha inválida");
//       }
//     }
//     else if(!strcmp(choice, "$")) {
//       mvprintw(10+MARGIN,2+MARGIN,"");
//     }
//     else {
//       mvprintw(10+MARGIN,2+MARGIN,"Escolha inválida");
//     }
// }
