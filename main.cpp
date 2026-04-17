#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROZMIAR 6

void Stworzplansze(int plansza[ROZMIAR][ROZMIAR]) {
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
    int Wylosujgdzie = rand() % liczpuste;
    int wiersz = puste[Wylosujgdzie][0];
    int kolumna = puste[Wylosujgdzie][1];

    plansza[wiersz][kolumna] = 2;
}

void Wygenerujplansze(int plansza[ROZMIAR][ROZMIAR]) {
    for (int i = 0; i < ROZMIAR; i++) {
        for (int j = 0; j < ROZMIAR; j++) {
            printf("%7d ", plansza[i][j]);
        }
        printf("\n\n");
    }
}

int main() {
    int plansza[ROZMIAR][ROZMIAR];
    srand(time(NULL));

    Stworzplansze(plansza);
    WylosujPole(plansza);
    WylosujPole(plansza);

    Wygenerujplansze(plansza);
    return 0;
}
