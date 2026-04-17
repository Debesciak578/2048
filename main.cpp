#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

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
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(600, 400);
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;
    bool redrawFrame = true;

    al_start_timer(timer);
    while (true) {
        al_wait_for_event(event_queue, &event);

        if (event.type = ALLEGRO_EVENT_TIMER) redrawFrame = true;

        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)) break;

        if (redrawFrame && al_is_event_queue_empty(event_queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_flip_display();

            redrawFrame = false;
        }
    }


    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    int plansza[ROZMIAR][ROZMIAR];
    srand(time(NULL));

    StworzPlansze(plansza);
    WylosujPole(plansza);
    WylosujPole(plansza);

    WygenerujPlansze(plansza);
    return 0;
}
