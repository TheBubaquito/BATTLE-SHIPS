#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.c>
#include <windows.h>

#define TAM                         10

#define ENTER                       13
#define ESC                         27
#define ARRIBA                      72
#define ABAJO                       80
#define IZQUIERDA                   75
#define DERECHA                     77

#define VER                         'v'

#define POSX                        30
#define POSY                        5
#define ESP                         1

#define MAX_BARCOS                  5

#define POS_MENSAJE_X               65
#define POS_MENSAJE_Y               7

#define CELDA_VACIA                 ' '
#define BARCO_JUGADOR               'B'
#define BARCO_CPU                   'C'
#define IMPACTO_FALLIDO             '*'
#define BARCO_JUGADOR_HUNDIDO       'X'
#define BARCO_CPU_HUNDIDO           '#'

#define COLOR_SELEC_TEXT            BLACK
#define COLOR_SELEC_FONDO           YELLOW

#define COLOR_FALLO_TEXT            BLACK
#define COLOR_FALLO_FONDO           LIGHTGRAY

#define COLOR_TABLERO_TEXT          BLACK
#define COLOR_TABLERO_FONDO         WHITE

#define COLOR_WIN                   GREEN
#define COLOR_WIN_BACK              BLACK

#define COLOR_LOSS                  RED
#define COLOR_LOSS_BACK             BLACK



void setcolor(int ct, int cf);
void colordefault();
void limpiarLinea(int y);
void delay(int mensajes);
void inicializarTablero(char tablero[TAM][TAM]);
void mostrarTablero(char tablero[TAM][TAM], int filaSel, int colSel, int mostrar, int barcos_jugador, int barcos_computadora);
void colocarBarcosJugador(char tablero[TAM][TAM], int* barcos_jugador);
void colocarBarcosCPU(char tablero[TAM][TAM]);
int turnoJugador(char tablero[TAM][TAM], int *barcos_jugador, int *barcos_computadora, int mostrar_barcos);
int turnoCPU(char tablero[TAM][TAM], int *barcos_jugador);
int juegoTerminado(int barcos_jugador, int barcos_computadora);


int main()
{
    char tablero[TAM][TAM];
    int barcos_jugador = MAX_BARCOS;
    int barcos_computadora = MAX_BARCOS;
    int mostrar_barcos = 0;

    srand(time(NULL));
    _setcursortype(0);
    inicializarTablero(tablero);
    colocarBarcosJugador(tablero, &barcos_jugador);
    colocarBarcosCPU(tablero);

    while (!juegoTerminado(barcos_jugador,  barcos_computadora))
    {
        mostrar_barcos = turnoJugador(tablero, &barcos_jugador, &barcos_computadora, mostrar_barcos);

        if (juegoTerminado(barcos_jugador, barcos_computadora)) break;


        turnoCPU(tablero, &barcos_jugador);
        Sleep(500);
    }

    mostrarTablero(tablero, -1, -1, 1, barcos_jugador, barcos_computadora);
    gotoxy(POSX, POSY + TAM + 4);
    if (barcos_jugador > 0)
    {
        setcolor(COLOR_WIN, COLOR_WIN_BACK);
        printf("Ganaste");
    }
    else
    {
        setcolor(COLOR_LOSS, COLOR_LOSS_BACK);
        printf("La computadora gano");
    }

    gotoxy(POSX, POSY + TAM + 5);
    printf("Barcos restantes - Jugador: %d | CPU: %d", barcos_jugador, barcos_computadora);

    _setcursortype(0);
    return 0;
}

/*
   Función: setcolor
   Argumentos: int ct: color del texto
               int cf: color de fondo
   Objetivo: establecer el color del texto y fondo en consola
   Retorno: ninguno
*/

void setcolor(int ct, int cf)
{
    textcolor(ct);
    textbackground(cf);
}

/*
   Función: colordefault
   Argumentos: ninguno
   Objetivo: restablecer los colores por defecto de la consola
   Retorno: ninguno
*/
void colordefault()
{
    setcolor(LIGHTGRAY, BLACK);
}

/*
   Función: limpiarLinea
   Argumentos: int y: posición vertical donde se limpia la línea
   Objetivo: limpiar una línea completa de la consola
   Retorno: ninguno
*/
void limpiarLinea(int y)
{
    gotoxy(0, y);
    for (int i = 0; i < 80; i++) printf(" ");
}
/*
   Función: delay
   Argumentos: int mensajes: cantidad de milisegundos de pausa
   Objetivo: pausar la ejecución por un tiempo determinado
   Retorno: ninguno
*/

void delay(int mensajes)
{
    Sleep(mensajes);
}

/*
   Función: inicializarTablero
   Argumentos: char tablero[TAM][TAM]: matriz del tablero de juego
   Objetivo: inicializar el tablero con celdas vacías
   Retorno: ninguno
*/

void inicializarTablero(char tablero[TAM][TAM])
{
    for (int fila = 0; fila < TAM; fila++)
        for (int col = 0; col < TAM; col++)
            tablero[fila][col] = CELDA_VACIA;
}
/*
   Función: mostrarTablero
   Argumentos: char tablero[TAM][TAM]: matriz del tablero
               int filaSel: fila seleccionada
               int colSel: columna seleccionada
               int mostrar: indica si se muestran los barcos
               int barcos_jugador: barcos restantes del jugador
               int barcos_computadora: barcos restantes de la CPU
   Objetivo: mostrar el estado actual del tablero en consola
   Retorno: ninguno
*/

void mostrarTablero(char tablero[TAM][TAM], int filaSel, int colSel, int mostrar, int barcos_jugador, int barcos_computadora)
{
    int x, y;
    for (int fila = 0; fila < TAM; fila++)
      {
        for (int col = 0; col < TAM; col++)
        {
            x = POSX + col * ESP;
            y = POSY + fila * ESP;
            gotoxy(x, y);

            setcolor(COLOR_TABLERO_TEXT, COLOR_TABLERO_FONDO);

            if (fila == filaSel && col == colSel)
                setcolor(COLOR_SELEC_TEXT, COLOR_SELEC_FONDO);
            else if (tablero[fila][col] == BARCO_JUGADOR_HUNDIDO)
                setcolor(RED, COLOR_TABLERO_FONDO);
            else if (tablero[fila][col] == BARCO_CPU_HUNDIDO)
                setcolor(GREEN, COLOR_TABLERO_FONDO);
            else if (tablero[fila][col] == IMPACTO_FALLIDO)
                setcolor(COLOR_FALLO_TEXT, COLOR_FALLO_FONDO);
            else
                setcolor(COLOR_TABLERO_TEXT, COLOR_TABLERO_FONDO);


            if (!mostrar && (tablero[fila][col] == BARCO_JUGADOR || tablero[fila][col] == BARCO_CPU))
                printf(" ");
            else
                printf("%c", tablero[fila][col]);
            colordefault();
        }
      }

    gotoxy(POSX, POSY + TAM + 2);
    colordefault();
    printf("Tus barcos: %d  |  Barcos de la computadora: %d", barcos_jugador, barcos_computadora);
}

/*
   Función: colocarBarcosJugador
   Argumentos: char tablero[TAM][TAM]: tablero del juego
               int* barcos_jugador: puntero a la cantidad de barcos del jugador
   Objetivo: permitir al jugador colocar sus barcos en el tablero
   Retorno: ninguno
*/

void colocarBarcosJugador(char tablero[TAM][TAM], int* barcos_jugador)
{
    int fila = 0, col = 0, colocados = 0;
    char tecla;
    mostrarTablero(tablero, fila, col, 1, MAX_BARCOS, MAX_BARCOS);

    do
      {
        limpiarLinea(POSY + TAM + 1);
        gotoxy(POSX, POSY + TAM + 1);
        printf("Coloca tus barcos: %d de %d", colocados, MAX_BARCOS);

        tecla = getch();

        if (tecla == ENTER) {
            if (colocados == MAX_BARCOS) {
                break;
            }

            if (tablero[fila][col] == BARCO_JUGADOR) {
                tablero[fila][col] = CELDA_VACIA;
                colocados--;
            } else if (tablero[fila][col] == CELDA_VACIA && colocados < MAX_BARCOS) {
                tablero[fila][col] = BARCO_JUGADOR;
                colocados++;
            }
        } else if (tecla == ARRIBA) fila = (fila - 1 + TAM) % TAM;
        else if (tecla == ABAJO) fila = (fila + 1) % TAM;
        else if (tecla == IZQUIERDA) col = (col - 1 + TAM) % TAM;
        else if (tecla == DERECHA) col = (col + 1) % TAM;

        mostrarTablero(tablero, fila, col, 1, MAX_BARCOS, MAX_BARCOS);
      } while (1);

    *barcos_jugador = colocados;

    limpiarLinea(POSY + TAM + 3);
    gotoxy(POSX, POSY + TAM + 3);
    printf("Presiona ENTER para confirmar ubicacion de barcos y empezar juego ...");

    while (getch() != ENTER);
}

/*
   Función: colocarBarcosCPU
   Argumentos: char tablero[TAM][TAM]: tablero del juego
   Objetivo: colocar aleatoriamente los barcos de la CPU en el tablero
   Retorno: ninguno
*/

void colocarBarcosCPU(char tablero[TAM][TAM])
{
    int fila, col, colocados = 0;
    system("cls");
    setcolor(LIGHTGRAY, BLACK);
    gotoxy(POSX, POSY);
    printf("La computadora esta desplegando los barcos:");
    while (colocados < MAX_BARCOS)
      {
        fila = rand() % TAM;
        col = rand() % TAM;
        if (tablero[fila][col] == CELDA_VACIA)
         {
            tablero[fila][col] = BARCO_CPU;
            gotoxy(POSX, POSY + 1 + colocados);
            printf("Barco %d desplegado...", colocados + 1);
            colocados++;
            Sleep(500);
         }
      }
    Sleep(500);
    system("cls");
}


/*
   Función: turnoJugador
   Argumentos: char tablero[TAM][TAM]: tablero del juego
               int* barcos_jugador: puntero a los barcos del jugador
               int* barcos_computadora: puntero a los barcos de la CPU
               int mostrar_barcos: bandera para mostrar barcos
   Objetivo: gestionar el turno del jugador, incluyendo disparos y visualización
   Retorno: (int) estado de la bandera mostrar_barcos (0 o 1)
*/

int turnoJugador(char tablero[TAM][TAM], int *barcos_jugador, int *barcos_computadora, int mostrar_barcos)
{
    int fila = 0, col = 0;
    char tecla;

    mostrarTablero(tablero, fila, col, mostrar_barcos, *barcos_jugador, *barcos_computadora);

    do
      {
        limpiarLinea(POSY + TAM + 1);
        gotoxy(POSX, POSY + TAM + 1);
        printf("Tu turno - ENTER para disparar  |  Tecla V para ver mapa ");
        tecla = getch();

        if (tecla == VER || tecla == 'V')
         {
            mostrar_barcos = !mostrar_barcos;
            mostrarTablero(tablero, fila, col, mostrar_barcos, *barcos_jugador, *barcos_computadora);
         } else if (tecla == ENTER)
         {
            if (tablero[fila][col] == BARCO_CPU)
            {
                tablero[fila][col] = BARCO_CPU_HUNDIDO;
                (*barcos_computadora)--;
                gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y);
                printf("Jugador Impacto exitoso en barco enemigo");
                break;
            } else if (tablero[fila][col] == BARCO_JUGADOR)
            {
                tablero[fila][col] = BARCO_JUGADOR_HUNDIDO;
                (*barcos_jugador)--;
                gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y);
                printf("Jugador disparo a su propio barco...");
                break;
            } else if (tablero[fila][col] == CELDA_VACIA)
            {
                tablero[fila][col] = IMPACTO_FALLIDO;
                gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y);
                printf("Jugador fallo el disparo");
                break;
            } else
            {
                gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y);
                printf("Ya disparaste ahi. Intenta otro lugar.   ");
            }
        } else if (tecla == ARRIBA) fila = (fila - 1 + TAM) % TAM;
        else if (tecla == ABAJO) fila = (fila + 1) % TAM;
        else if (tecla == IZQUIERDA) col = (col - 1 + TAM) % TAM;
        else if (tecla == DERECHA) col = (col + 1) % TAM;

        mostrarTablero(tablero, fila, col, mostrar_barcos, *barcos_jugador, *barcos_computadora);
      } while (1);
       mostrarTablero(tablero, fila, col, mostrar_barcos, *barcos_jugador, *barcos_computadora);
    return mostrar_barcos;
}

/*
   Función: turnoCPU
   Argumentos: char tablero[TAM][TAM]: tablero del juego
               int* barcos_jugador: puntero a los barcos del jugador
   Objetivo: gestionar el turno de ataque de la CPU
   Retorno: (int) siempre 0 (puede modificarse para retorno útil)
*/

int turnoCPU(char tablero[TAM][TAM], int *barcos_jugador)
{
    int fila, col;

    do
      {
        fila = rand() % TAM;
        col = rand() % TAM;
      } while (tablero[fila][col] == IMPACTO_FALLIDO || tablero[fila][col] == BARCO_JUGADOR_HUNDIDO || tablero[fila][col] == BARCO_CPU_HUNDIDO);

    if (tablero[fila][col] == BARCO_JUGADOR)
      {
        tablero[fila][col] = BARCO_JUGADOR_HUNDIDO;
        (*barcos_jugador)--;
        gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y + 1);
        printf("Computadora impacto un barco tuyo.      ");
      } else if (tablero[fila][col] == BARCO_CPU)
      {
        tablero[fila][col] = BARCO_CPU_HUNDIDO;
        gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y + 1);
        printf("Computadora se disparo a si misma.     ");
      } else
      {
        tablero[fila][col] = IMPACTO_FALLIDO;
        gotoxy(POS_MENSAJE_X, POS_MENSAJE_Y + 1);
        printf("Computadora fallo el disparo.           ");
      }

    return 0;
}

/*
   Función: juegoTerminado
   Argumentos: int barcos_jugador: cantidad de barcos del jugador
               int barcos_computadora: cantidad de barcos de la CPU
   Objetivo: verificar si el juego ha terminado
   Retorno: (int) 1 si uno de los jugadores ya no tiene barcos, 0 si no
*/

int juegoTerminado(int barcos_jugador, int barcos_computadora)
{
    return (barcos_jugador <= 0 || barcos_computadora <= 0);
}
