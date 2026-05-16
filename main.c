#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>




#define GRID_SIZE 4
    
typedef struct { //struktura jednej komorki, zawiera liczbe, kolor oraz wspolrzedne rogow tej komorki i jej tekstu
    int number;
    float x1, y1, x2, y2, textX, textY;
    char numberInCharFormat[8];
    ALLEGRO_COLOR color;
} Cell;


void moveGridRight(Cell grid[GRID_SIZE][GRID_SIZE], int* maxNumber, int* pointCounter) {
    for (int i = 0; i < GRID_SIZE; i++) {

        // krok 1 - przesuwamy wszystko w prawo
        for (int j = GRID_SIZE - 2; j >= 0; j--) {
            if (grid[j][i].number == 0) continue;

            int cur = j;
            while (cur + 1 < GRID_SIZE && grid[cur + 1][i].number == 0) {
                grid[cur + 1][i].number = grid[cur][i].number;
                grid[cur + 1][i].color = grid[cur][i].color;
                grid[cur][i].number = 0;
                grid[cur][i].color = al_map_rgb(255, 255, 255);
                cur++;
            }
        }

        // krok 2 - polaczamy jednakowe komorki
        for (int j = GRID_SIZE - 1; j > 0; j--) {
            if (grid[j][i].number == 0) continue;
            if (grid[j][i].number == grid[j - 1][i].number) {
                grid[j][i].number *= 2;
                grid[j - 1][i].number = 0;
                grid[j - 1][i].color = al_map_rgb(255, 255, 255);

            }
        }

        // krok 3 - znowu przesuwamy wszystko w prawo zeby pozbyc sie dziur
        for (int j = GRID_SIZE - 2; j >= 0; j--) {
            if (grid[j][i].number == 0) continue;

            int cur = j;
            while (cur + 1 < GRID_SIZE && grid[cur + 1][i].number == 0) {
                grid[cur + 1][i].number = grid[cur][i].number;
                grid[cur + 1][i].color = grid[cur][i].color;
                grid[cur][i].number = 0;
                grid[cur][i].color = al_map_rgb(255, 255, 255);
                cur++;
            }
        }
    }
}


void moveGridLeft(Cell grid[GRID_SIZE][GRID_SIZE], int* maxNumber, int* pointCounter) {
    for (int i = 0; i < GRID_SIZE; i++) {

        // krok 1 - przesuwamy wszystko w lewo
        for (int j = 2; j < GRID_SIZE; j++) {
            if (grid[j][i].number == 0) continue;

            int cur = j;
            while (cur - 1 >= 0 && grid[cur - 1][i].number == 0) {
                grid[cur - 1][i].number = grid[cur][i].number;
                grid[cur - 1][i].color = grid[cur][i].color;
                grid[cur][i].number = 0;
                grid[cur][i].color = al_map_rgb(255, 255, 255);
                cur--;
            }
        }

        // krok 2 - polaczamy jednakowe komorki
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[j][i].number == 0) continue;
            if (grid[j][i].number == grid[j + 1][i].number) {
                grid[j][i].number *= 2;
                grid[j + 1][i].number = 0;
                grid[j + 1][i].color = al_map_rgb(255, 255, 255);

            }
        }

        // krok 3 - znowu przesuwamy wszystko w lewo zeby pozbyc sie dziur
        for (int j = 1; j < GRID_SIZE; j++) {
            if (grid[j][i].number == 0) continue;

            int cur = j;
            while (cur - 1 >= 0 && grid[cur - 1][i].number == 0) {
                grid[cur - 1][i].number = grid[cur][i].number;
                grid[cur - 1][i].color = grid[cur][i].color;
                grid[cur][i].number = 0;
                grid[cur][i].color = al_map_rgb(255, 255, 255);
                cur--;
            }
        }
    }
}

void moveGridUp(Cell grid[GRID_SIZE][GRID_SIZE], int* maxNumber, int* pointCounter) {
    for (int i = 0; i < GRID_SIZE; i++) {

        for (int j = 1; j < GRID_SIZE; j++) {
            if (grid[i][j].number == 0) continue;

            int cur = j;
            while (cur - 1 >= 0 && grid[i][cur - 1].number == 0) {
                grid[i][cur - 1].number = grid[i][cur].number;
                grid[i][cur - 1].color = grid[i][cur].color;
                grid[i][cur].number = 0;
                grid[i][cur].color = al_map_rgb(255, 255, 255);
                cur--;
            }
        }

        for (int j = 0; j < GRID_SIZE - 1; j++) {
            if (grid[i][j].number == 0) continue;
            if (grid[i][j].number == grid[i][j + 1].number) {
                grid[i][j].number *= 2;
                grid[i][j + 1].number = 0;
                grid[i][j + 1].color = al_map_rgb(255, 255, 255);

            }
        }

        for (int j = 1; j < GRID_SIZE; j++) {
            if (grid[i][j].number == 0) continue;

            int cur = j;
            while (cur - 1 >= 0 && grid[i][cur - 1].number == 0) {
                grid[i][cur - 1].number = grid[i][cur].number;
                grid[i][cur - 1].color = grid[i][cur].color;
                grid[i][cur].number = 0;
                grid[i][cur].color = al_map_rgb(255, 255, 255);
                cur--;
            }
        }
    }
}

void moveGridDown(Cell grid[GRID_SIZE][GRID_SIZE], int *maxNumber, int *pointCounter) {
    for (int i = 0; i < GRID_SIZE; i++) {

        for (int j = GRID_SIZE - 2; j >= 0; j--) {
            if (grid[i][j].number == 0) continue;

            int cur = j;
            while (cur + 1 < GRID_SIZE && grid[i][cur + 1].number == 0) {
                grid[i][cur + 1].number = grid[i][cur].number;
                grid[i][cur + 1].color = grid[i][cur].color;
                grid[i][cur].number = 0;
                grid[i][cur].color = al_map_rgb(255, 255, 255);
                cur++;
            }
        }

        for (int j = GRID_SIZE - 1; j > 0; j--) {
            if (grid[i][j].number == 0) continue;

            if (grid[i][j].number == grid[i][j - 1].number) {
                grid[i][j].number *= 2;
                grid[i][j - 1].number = 0;
                grid[i][j - 1].color = al_map_rgb(255, 255, 255);

            }
        }

        for (int j = GRID_SIZE - 2; j >= 0; j--) {
            if (grid[i][j].number == 0) continue;

            int cur = j;
            while (cur + 1 < GRID_SIZE && grid[i][cur + 1].number == 0) {
                grid[i][cur + 1].number = grid[i][cur].number;
                grid[i][cur + 1].color = grid[i][cur].color;
                grid[i][cur].number = 0;
                grid[i][cur].color = al_map_rgb(255, 255, 255);
                cur++;
            }
        }
    }
}


void CreateGrid(Cell grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].number = 0;
            grid[i][j].x1 = 105 + i * 150;
            grid[i][j].y1 = 105 + j * 150;
            grid[i][j].x2 = 245 + i * 150;
            grid[i][j].y2 = 245 + j * 150;
            grid[i][j].textX = 170 + i * 150;
            grid[i][j].textY = 170 + j * 150;
            grid[i][j].color = al_map_rgb(255, 255, 255);
        }
    }
}

void CalculateAndFillRandomCell(Cell grid[GRID_SIZE][GRID_SIZE]) {
    int empty[GRID_SIZE * GRID_SIZE][2];
    int empty_counter = 0;

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].number == 0) {
                empty[empty_counter][0] = i;
                empty[empty_counter][1] = j;
                empty_counter++;
            }
        }
    }

    int generateRandomPoint = rand() % empty_counter;
    int row = empty[generateRandomPoint][0];
    int column = empty[generateRandomPoint][1];

    grid[row][column].number = 2;
}

void DrawGrid(Cell grid[GRID_SIZE][GRID_SIZE], ALLEGRO_FONT* font, int pointNumber, int maxNumber, char pointCounterInChar[], char maxNumberInChar[]) {
    sprintf(pointCounterInChar, "%d", pointNumber);
    sprintf(maxNumberInChar, "%d", maxNumber);
    al_draw_text(font, al_map_rgb(0, 0, 0), 80, 30, ALLEGRO_ALIGN_CENTER,  pointCounterInChar);
    al_draw_text(font, al_map_rgb(0, 0, 0), 80, 150, ALLEGRO_ALIGN_CENTER, maxNumberInChar);
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            sprintf(grid[i][j].numberInCharFormat, "%d", grid[i][j].number);
            al_draw_filled_rounded_rectangle(grid[i][j].x1, grid[i][j].y1, grid[i][j].x2, grid[i][j].y2, 15, 15, grid[i][j].color);
            if(grid[i][j].number != 0) al_draw_text(font, al_map_rgb(0,0,0), grid[i][j].textX, grid[i][j].textY, ALLEGRO_ALIGN_CENTRE, grid[i][j].numberInCharFormat);
        }
    }
}

int main() {
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(40);

    ALLEGRO_DISPLAY* display = al_create_display(GRID_SIZE * 150 + 200, GRID_SIZE * 150 + 200);
    ALLEGRO_FONT* font = al_load_ttf_font("Roboto.ttf", 40, 0);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    ALLEGRO_SAMPLE* click_sound = al_load_sample("discord.mp3");


    ALLEGRO_EVENT event;
    int redrawFrame = 1;

    int pointCounter = 0;
    int maxNumber = 0;
    int* pointPTR = &pointCounter;
    int* maxPTR = &maxNumber;
    char pointCounterInChar[12];
    char maxNumberInChar[12];

    Cell grid[GRID_SIZE][GRID_SIZE];
    srand(time(NULL));

    CreateGrid(grid);
    al_start_timer(timer);

    void (*moveType)(Cell grid[GRID_SIZE][GRID_SIZE], int* maxNumber, int* pointCounter);
    moveType = moveGridRight;

    while (true) {
        al_wait_for_event(event_queue, &event);


        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            redrawFrame = 1;
            switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_RIGHT:
                moveType = &moveGridRight;
                break;
            case ALLEGRO_KEY_LEFT:
                moveType = &moveGridLeft;
                break;
            case ALLEGRO_KEY_UP:
                moveType = &moveGridUp;
                break;
            case ALLEGRO_KEY_DOWN:
                moveType = &moveGridDown;
                break;
            default:
                redrawFrame = 0;
            }
            if (redrawFrame && click_sound) {
                al_play_sample(click_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }

        if (redrawFrame && al_is_event_queue_empty(event_queue))
        {
            al_clear_to_color(al_map_rgb(255, 204, 137));

            moveType(grid, maxPTR, pointPTR);
            pointCounter = 0;
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    pointCounter += grid[i][j].number;
                    if (maxNumber < grid[i][j].number) maxNumber = grid[i][j].number;
                }
            }
            CalculateAndFillRandomCell(grid);

            DrawGrid(grid, font, pointCounter, maxNumber, pointCounterInChar, maxNumberInChar);
            al_flip_display();

            redrawFrame = false;
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
    }
    if (click_sound) al_destroy_sample(click_sound);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
