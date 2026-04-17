#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#define ROZMIAR 6
//test 123

void StworzPlansze(int plansza[ROZMIAR][ROZMIAR]) {
    for (int i = 0; i < ROZMIAR; i++) {
        for (int j = 0; j < ROZMIAR; j++) {
            plansza[i][j] = 0;
        }
    }
}

void WylosujPole(int plansza[ROZMIAR][ROZMIAR]) {
    int puste[ROZMIAR * ROZMIAR][2];
    int liczpuste = 0;

    for (int i = 0; i < ROZMIAR; i++) {
        for (int j = 0; j < ROZMIAR; j++) {
            if (plansza[i][j] == 0) {
                puste[liczpuste][0] = i;
                puste[liczpuste][1] = j;
                liczpuste++;
            }
        }
    }
    int WylosujGdzie = rand() % liczpuste;
    int wiersz = puste[WylosujGdzie][0];
    int kolumna = puste[WylosujGdzie][1];

    plansza[wiersz][kolumna] = 2;
}

void WygenerujPlansze(int plansza[ROZMIAR][ROZMIAR]) {
    for (int i = 0; i < ROZMIAR; i++) {
        for (int j = 0; j < ROZMIAR; j++) {
            printf("%7d ", plansza[i][j]);
        }
        printf("\n\n");
    }
}

int main() {
    al_init();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(600, 400);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    int plansza[ROZMIAR][ROZMIAR];
    srand(time(NULL));

    StworzPlansze(plansza);
    WylosujPole(plansza);
    WylosujPole(plansza);

    WygenerujPlansze(plansza);
    return 0;
}
