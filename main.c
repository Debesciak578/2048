#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>



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

void moveGridDown(Cell grid[GRID_SIZE][GRID_SIZE], int* maxNumber, int* pointCounter) {
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
	if (empty_counter == 0) return;
	int generateRandomPoint = rand() % empty_counter;
	int row = empty[generateRandomPoint][0];
	int column = empty[generateRandomPoint][1];

	grid[row][column].number = 2;
}
//sprawdzenie czy nie ma mozliwosci ruchu
bool checkLoss(Cell grid[GRID_SIZE][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (grid[i][j].number == 0) return false;
			if (i + 1 < GRID_SIZE && grid[i][j].number == grid[i + 1][j].number) return false;
			if (j + 1 < GRID_SIZE && grid[i][j].number == grid[i][j + 1].number) return false;
		}
	}
	return true;
}

void DrawGrid(Cell grid[GRID_SIZE][GRID_SIZE], ALLEGRO_FONT* font, int pointNumber, int maxNumber, char pointCounterInChar[], char maxNumberInChar[]) {
	sprintf(pointCounterInChar, "%d", pointNumber);
	sprintf(maxNumberInChar, "%d", maxNumber);
	al_draw_text(font, al_map_rgb(255, 255, 255), 80, 30, ALLEGRO_ALIGN_CENTER, pointCounterInChar);
	al_draw_text(font, al_map_rgb(255, 255, 255), 80, 150, ALLEGRO_ALIGN_CENTER, maxNumberInChar);
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			sprintf(grid[i][j].numberInCharFormat, "%d", grid[i][j].number);
			al_draw_filled_rounded_rectangle(grid[i][j].x1, grid[i][j].y1, grid[i][j].x2, grid[i][j].y2, 15, 15, grid[i][j].color);
			if (grid[i][j].number != 0) al_draw_text(font, al_map_rgb(0, 0, 0), grid[i][j].textX, grid[i][j].textY, ALLEGRO_ALIGN_CENTRE, grid[i][j].numberInCharFormat);
		}
	}
}

void RestartGame(Cell grid[GRID_SIZE][GRID_SIZE], int* pointCounter, int* maxNumber, bool* gameOver, bool* gameWon,
	bool* alreadyWon, bool* canUndo, int* previousPointCounter, int* previousMaxNumber)
{
	CreateGrid(grid);                         
	CalculateAndFillRandomCell(grid);  

	*pointCounter = 0;
	*maxNumber = 0;
	*previousPointCounter = 0;
	*previousMaxNumber = 0;
	*gameOver = false;
	*gameWon = false;
	*alreadyWon = false;
	*canUndo = false;
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
	al_init_image_addon();

	ALLEGRO_DISPLAY* display = al_create_display(GRID_SIZE * 150 + 200, GRID_SIZE * 150 + 200);
	ALLEGRO_FONT* font = al_load_ttf_font("Roboto.ttf", 40, 0);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	ALLEGRO_SAMPLE* click_sound = al_load_sample("discord.mp3");
	ALLEGRO_BITMAP* background_img = al_load_bitmap("backg.png");


	ALLEGRO_EVENT event;
	int redrawFrame = 1;

	int pointCounter = 0;
	int maxNumber = 0;
	int* pointPTR = &pointCounter;
	int* maxPTR = &maxNumber;
	char pointCounterInChar[12];
	char maxNumberInChar[12];
	//zmienne do UNDO
	Cell previousGrid[GRID_SIZE][GRID_SIZE];
	int previousPointCounter = 0;
	int previousMaxNumber = 0;
	bool canUndo = false;

	bool gameOver = false;
	bool gameWon = false;
	bool alreadyWon = false;
	bool skipLogic = false;

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
				if (gameOver || gameWon) { redrawFrame = 0; break; } //zablokowa³em ruszanie sie jak jest menu 
				moveType = &moveGridRight;
				break;
			case ALLEGRO_KEY_LEFT:
				if (gameOver || gameWon) { redrawFrame = 0; break; }
				moveType = &moveGridLeft;
				break;
			case ALLEGRO_KEY_UP:
				if (gameOver || gameWon) { redrawFrame = 0; break; }
				moveType = &moveGridUp;
				break;
			case ALLEGRO_KEY_DOWN:
				if (gameOver || gameWon) { redrawFrame = 0; break; }
				moveType = &moveGridDown;
				break;
			case ALLEGRO_KEY_C://cofanie guzikiem c
				if (gameOver || gameWon) { redrawFrame = 0; break; } //blokada cofniecia
				moveType = NULL;
				break;
			case ALLEGRO_KEY_SPACE:
				if (gameWon) {
					gameWon = false;
					alreadyWon = true;
					skipLogic = true;
				}
				else {
					redrawFrame = 0;
				}
				break;
			case ALLEGRO_KEY_R: //restart gry przy nacisnieciu na R
				RestartGame(grid, pointPTR, maxPTR, &gameOver, &gameWon, &alreadyWon, &canUndo, &previousPointCounter, &previousMaxNumber);
				skipLogic = true;
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
			int sizee = GRID_SIZE * 150 + 200;
			if (background_img) {
				al_draw_scaled_bitmap(background_img, 0, 0, al_get_bitmap_width(background_img), al_get_bitmap_height(background_img), 0, 0, sizee, sizee, 0);
			}
			else {
				al_clear_to_color(al_map_rgb(255, 204, 137));
			}

			if (!skipLogic) {
				if (moveType == NULL) {
					//podmiana na stare wartosci
					if (canUndo) {
						for (int i = 0; i < GRID_SIZE; i++)
							for (int j = 0; j < GRID_SIZE; j++) grid[i][j] = previousGrid[i][j];
						pointCounter = previousPointCounter;
						maxNumber = previousMaxNumber;
						canUndo = false;
					}
				}
				else {
					if (!gameOver && !gameWon) {
						//zapis planszy odpamieci
						for (int i = 0; i < GRID_SIZE; i++)
							for (int j = 0; j < GRID_SIZE; j++) previousGrid[i][j] = grid[i][j];
						previousPointCounter = pointCounter;
						previousMaxNumber = maxNumber;
						canUndo = true;


						moveType(grid, maxPTR, pointPTR);
						pointCounter = 0;
						for (int i = 0; i < GRID_SIZE; i++) {
							for (int j = 0; j < GRID_SIZE; j++) {
								pointCounter += grid[i][j].number;
								if (maxNumber < grid[i][j].number) maxNumber = grid[i][j].number;
							}
						}
						CalculateAndFillRandomCell(grid);

						//sprawdzenie czy wygrana lub przegrana
						if (maxNumber >= 2048 && !alreadyWon) gameWon = true;
						if (checkLoss(grid)) gameOver = true;
					}
				}
			}
			skipLogic = false;

			DrawGrid(grid, font, pointCounter, maxNumber, pointCounterInChar, maxNumberInChar);

			//wypisanie ekranow przegranej i wygranej
			int size = GRID_SIZE * 150 + 200;
			if (gameOver) {
				al_draw_filled_rectangle(0, 0, size, size, al_map_rgba(0, 0, 0, 210));
				al_draw_text(font, al_map_rgb(255, 50, 50), size / 2, size / 2, ALLEGRO_ALIGN_CENTER, "PRZEGRANA!");
			}
			else if (gameWon) {
				al_draw_filled_rectangle(0, 0, size, size, al_map_rgba(237, 194, 46, 200));
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2 - 30, ALLEGRO_ALIGN_CENTER, "WYGRANA!");
			}


			al_flip_display();

			redrawFrame = false;
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}
	}
	if (background_img) al_destroy_bitmap(background_img);
	if (click_sound) al_destroy_sample(click_sound);
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	return 0;
}