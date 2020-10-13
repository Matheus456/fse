#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <poll.h>
#include <stdio.h>

#define MARGIN 2

void tempInfo(double *temp);
void menuBorders();
void menuUser(double *temp);

int main() 
// void *print_temperatures()
{
    int row,col;				/* to store the number of rows and *
    //                     * the number of colums of the screen */
    int c = 0;
    /* Init ncurses mode */
    initscr ();
    /* Hide cursor */
    curs_set (0);
    double temp[4] = {20, 24, 25, 0};

    while (c < 1000) {
        char str[80] = "";
        menuBorders();
        tempInfo(temp);
        menuUser(temp);
        usleep(500000);
        refresh ();
        clear();
    }
    /* End ncurses mode */
    endwin();
    return 0;
}

void tempInfo(double *temp) {
    mvprintw (2+MARGIN, 65+MARGIN, "Informações do sistema");
    mvprintw (6+MARGIN, 60+MARGIN, "Temperatura Interior: %0.2lf", temp[0]++);
    mvprintw (7+MARGIN, 60+MARGIN, "Temperatura Exterior: %0.2lf", temp[1]++);
    mvprintw (8+MARGIN, 60+MARGIN, "Temperatura Referência: %0.2lf", temp[3]);
    mvprintw (9+MARGIN, 60+MARGIN, "Histeresse: %0.2lf", temp[3]);
}

void menuBorders() {
    char doubleBordX[200] = {"==================================================================================================="};
    char singleBordx[200] = {"---------------------------------------------------------------------------------------------------"};
    mvprintw (0+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (4+MARGIN, 1+MARGIN, doubleBordX);
    mvprintw (14+MARGIN, 1+MARGIN, singleBordx);
    for(int i=1; i<14; i++) {
      mvprintw (i+MARGIN, 0+MARGIN, "|");
      mvprintw (i+MARGIN, 50+MARGIN, "|");
      mvprintw (i+MARGIN, 100+MARGIN, "|");
    }
}

void menuUser(double *temp) {
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    char choice = '$';
    char breakline;
    char controlType;
    mvprintw (2+MARGIN, 20+MARGIN, "Menu do Sistema");
    mvprintw(5+MARGIN,2+MARGIN,"a. Alterar temperatura");
    mvprintw(6+MARGIN,2+MARGIN,"b. Alterar histerese");
    mvprintw(7+MARGIN,2+MARGIN,"c. Alterar tipo de controle");
    mvprintw(8+MARGIN,2+MARGIN,"d. Desligar sistema");

    mvprintw(9+MARGIN,2+MARGIN,"Digite a sua escolha:");
    if( poll(&mypoll, 1, 100) ){
      choice = getch();
      breakline = getch();
      mvprintw(20+MARGIN,2+MARGIN, "Choice %c", choice);
    }

    if(choice == 'a') {
      mvprintw(11+MARGIN,2+MARGIN,"Digite a temperatura: ");
      scanw(" %4lf", &temp[3]) ;
    }
    else if(choice == 'b') {
      mvprintw(11+MARGIN,2+MARGIN,"Digite o valor da histerese: ");
      scanw(" %4lf", &temp[3]) ;
    }
    else if(choice == 'c') {
      mvprintw(11+MARGIN,2+MARGIN,"Digite o controle desejado");
      mvprintw(12+MARGIN,2+MARGIN,"(p -> Potenciômetro/ t -> teclado):");
      char controle = getch();
      breakline = getch();
      if (controle == 'p' || controle == 't') {
        mvprintw(13+MARGIN,2+MARGIN,"Controle alterado");
      }
      else {
        mvprintw(13+MARGIN,2+MARGIN,"Escolha inválida");
      }
    }
    else if(choice == 'd') {
      mvprintw(10+MARGIN,2+MARGIN,"ADIOS");
    }
    else if(choice == '$') {
      mvprintw(10+MARGIN,2+MARGIN,"");
    }
    else {
      mvprintw(10+MARGIN,2+MARGIN,"Escolha inválida");
    }
}
// Primeiro menu
// Digite a temperatura desejada
// Digite o histerese desejado
// Qual será o tipo de controle? (t -> teclado/p -> potênciometro)

// Menu usuário
// a. Alterar temperatura
// b. Alterar histerese
// c. Alterar tipo de controle
// d. Desligar sistema