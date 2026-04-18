#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>



#define GRID_SIZE 6

struct Cell {
    int number = 0;
    float x1 = 0, y1 = 0, x2 = 0, y2 = 0, textX = 0, textY = 0;
    char numberInCharFormat[8];
    ALLEGRO_COLOR color;
} grid;



void moveGridRight(Cell grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = GRID_SIZE-1; j >= 0; j--) {
            if (grid[i][j].number != 0 && j < GRID_SIZE-1) {

                while (grid[i][j + 1].number == 0) {
                    grid[i][j + 1].number = grid[i][j].number;
                    grid[i][j + 1].color = grid[i][j].color;
                }

                if (grid[i][j].number == grid[i][j + 1].number) {
                    grid[i][j + 1].number *= 2;
                    grid[i][j].number = 0;
                }
            }
        }
    }
}

void moveGridLeft(Cell grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j].number != 0 && grid[i][j + 1].number < GRID_SIZE) {
                grid[i][j + 1].number = grid[i][j].number;
                grid[i][j + 1].color = grid[i][j].color;
            }
        }
    }
}

void moveGridUp(Cell grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = GRID_SIZE - 1; j >= 0; j--) {
            if (grid[i][j].number != 0 && grid[i][j + 1].number < GRID_SIZE) {
                grid[i][j + 1].number = grid[i][j].number;
                grid[i][j + 1].color = grid[i][j].color;
            }
        }
    }
}

void moveGridDown(Cell grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = GRID_SIZE - 1; j >= 0; j--) {
            if (grid[i][j].number != 0 && grid[i][j + 1].number < GRID_SIZE) {
                grid[i][j + 1].number = grid[i][j].number;
                grid[i][j + 1].color = grid[i][j].color;
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

void DrawGrid(Cell grid[GRID_SIZE][GRID_SIZE], ALLEGRO_FONT* font) {

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

    ALLEGRO_DISPLAY* display = al_create_display(GRID_SIZE * 150 + 200, GRID_SIZE * 150 + 200);
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;
    bool redrawFrame = true;

    Cell grid[GRID_SIZE][GRID_SIZE];
    srand(time(NULL));

    CreateGrid(grid);
    al_start_timer(timer);
    while (true) {
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) redrawFrame = true;

        if (redrawFrame && al_is_event_queue_empty(event_queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            DrawGrid(grid, font);
            CalculateAndFillRandomCell(grid);

            al_flip_display();

            redrawFrame = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
