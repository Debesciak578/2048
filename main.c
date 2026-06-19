/*****************************************************************//**
 * \file   main.c
 * \brief  Plik zawierajacy wszystkie (oprocz biblioteki Allegro) naglowki, funkcje, zmienne i kod zrodlowy dla gry 2048
 *
 * \author Zespol "Zucyki": Bartek Anton Olgierd
 * \date   May 2026
 *********************************************************************/


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


 /**
  * Zmienna globalna ktora odpowiada za ilosc komorek w grze oraz rozmiar okna gry tworzonego przez Allegro (zmienic na zmienna lokalna?).
  */
#define GRID_SIZE 4

  /**
	  @struct Cell
	  @brief  Struktura pojedynczej komorki
	  @param number Wartosc komorki, na ktorej wykonujemy wszystkie obliczenia.
	  @param x1 Wspolrzedna x w lewym gornym rogu komorki, potrzebna dla poprawnego wyswietlenia komorki na ekranie gracza
	  @param y1 Wspolrzedna y w lewym gornym rogu komorki, potrzebna dla poprawnego wyswietlenia komorki na ekranie gracza
	  @param x2 Wspolrzedna x w prawym dolnym rogu komorki, potrzebna dla poprawnego wyswietlenia komorki na ekranie gracza
	  @param y2 Wspolrzedna y w prawym odlnym rogu komorki, potrzebna dla poprawnego wyswietlenia komorki na ekranie gracza
	  @param textX Wspolrzedna x, potrzebna dla wyznaczenia miejsca wyswietlenia wartosci zamieszczonej w parametrze number
	  @param texty Wspolrzedna y, potrzebna dla wyznaczenia miejsca dla wyswietlenia wartosci zamieszczonej w parametrze number
	  @param numberInCharFormat Wartosc komorki, konwertowana w typ char[] dla mozliwosci wyswietlenia tej komorki za pomoca Allegro
	  @param color Kolor tej komorki, zalezy od jej wartosci
  **/
typedef struct {
	int number;
	float x1, y1, x2, y2, textX, textY;
	char numberInCharFormat[8];
	ALLEGRO_COLOR color;
} Cell;

ALLEGRO_COLOR getCellColor(int value) {
	switch (value) {
	case 2:    return al_map_rgb(238, 228, 218);
	case 4:    return al_map_rgb(237, 224, 200);
	case 8:    return al_map_rgb(242, 177, 121);
	case 16:   return al_map_rgb(245, 149, 99);
	case 32:   return al_map_rgb(246, 124, 95);
	case 64:   return al_map_rgb(246, 94, 59);
	case 128:  return al_map_rgb(237, 207, 114);
	case 256:  return al_map_rgb(237, 204, 97);
	case 512:  return al_map_rgb(237, 200, 80);
	case 1024: return al_map_rgb(237, 197, 63);
	case 2048: return al_map_rgb(237, 194, 46);
	default:   return al_map_rgb(255, 255, 255);
	}
}


typedef enum Direction
{
	LEFT, RIGHT, UP, DOWN
};


void moveGrid(Cell grid[GRID_SIZE][GRID_SIZE], enum Direction direction, int* maxNumber, int* pointCounter)
{
	for (int line = 0; line < GRID_SIZE; line++)
	{
		int values[GRID_SIZE] = { 0 };

		// odczyt linii

		for (int i = 0; i < GRID_SIZE; i++)
		{
			int x, y;

			switch (direction) {
			case LEFT:
				x = i;
				y = line;
				break;

			case RIGHT:
				x = GRID_SIZE - 1 - i;
				y = line;
				break;

			case UP:
				x = line;
				y = i;
				break;

			case DOWN:
				x = line;
				y = GRID_SIZE - 1 - i;
				break;
			}

			values[i] = grid[x][y].number;
		}

		// krok 1 - kompresja

		int target = 0;

		for (int i = 0; i < GRID_SIZE; i++)
		{
			if (values[i] != 0)
			{
				int temp = values[i];
				values[i] = 0;
				values[target++] = temp;
			}
		}

		// krok 2 - laczenie

		for (int i = 0; i < GRID_SIZE - 1; i++)
		{
			if (values[i] != 0 &&
				values[i] == values[i + 1])
			{
				values[i] *= 2;
				values[i + 1] = 0;
			}
		}

		// krok 3 - ponowna kompresja

		target = 0;

		for (int i = 0; i < GRID_SIZE; i++)
		{
			if (values[i] != 0)
			{
				int temp = values[i];
				values[i] = 0;
				values[target++] = temp;
			}
		}

		// zapis do planszy

		for (int i = 0; i < GRID_SIZE; i++)
		{
			int x, y;

			switch (direction)
			{
			case LEFT:
				x = i;
				y = line;
				break;

			case RIGHT:
				x = GRID_SIZE - 1 - i;
				y = line;
				break;

			case UP:
				x = line;
				y = i;
				break;

			case DOWN:
				x = line;
				y = GRID_SIZE - 1 - i;
				break;
			}

			grid[x][y].number = values[i];

			if (values[i] == 0)
			{
				grid[x][y].color =
					al_map_rgb(255, 255, 255);
			}
			else
			{
				grid[x][y].color =
					getCellColor(values[i]);
			}
		}
	}
}


/**
 * @brief Funkcja dla przesuniecia komorek w prawo.
 * @details
 * Dziala w 4 kroki:
 *
 * 1 -> "otwiera" petle for ktora odpowiada za wierszy macierzy parametru Cell grid[][]
 *
 * 2 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * w petli while "przesuwa" wszystkie wartosci o jedna komorke w prawo, przypisujac elementy number
 * i color struktury Cell z komorki z indeksem cur do komorki cur + 1
 * ustawiajac number z komorki cur  na 0 i color komorki cur na 255, 255, 255, czyli kolor bialy
 *
 * 3 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * jesli komorki z indeksami j i j - 1 sa jednakowe, number komorki j - 1 ustawia sie na 0,
 * a number komorki j mnozy sie na 2
 *
 * 4 -> powtorzenie kroku 2 dla usuniecia dziur pomiedzy komorkami utworzonymi po polaczeniu jednakowych komorek
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \param maxNumber Zmienna wskazujaca na licznik maksymalnej wartosci obecnej na planszy dla jego podalszej zmiany za warunku zdobycia wartosci wyzszej od terazniejszego maksymuma (na razie nie uzywany)
 * \param pointCounter Zmienna wskazujaca na licznik punktow dla jego podalszego zwiekszenia (na razie nie uzywany)
 */
void moveGridRight(Cell grid[GRID_SIZE][GRID_SIZE], int* maxNumber, int* pointCounter)
{
	moveGrid(grid, RIGHT, maxNumber, pointCounter);
}

/**
 * @brief Funkcja dla przesuniecia komorek w lewo.
 * @details
 * Dziala w 4 kroki:
 *
 * 1 -> "otwiera" petle for ktora odpowiada za wierszy macierzy parametru Cell grid[][]
 *
 * 2 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * w petli while "przesuwa" wszystkie wartosci o jedna komorke w prawo, przypisujac elementy number
 * i color struktury Cell z komorki z indeksem cur do komorki cur - 1
 * ustawiajac number z komorki cur na 0 i color komorki cur na 255, 255, 255, czyli kolor bialy
 *
 * 3 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * jesli komorki z indeksami j i j + 1 sa jednakowe, number komorki j + 1 ustawia sie na 0,
 * a number komorki j mnozy sie na 2
 *
 * 4 -> powtorzenie kroku 2 dla usuniecia dziur pomiedzy komorkami utworzonymi po polaczeniu jednakowych komorek
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \param maxNumber Zmienna wskazujaca na licznik maksymalnej wartosci obecnej na planszy dla jego podalszej zmiany za warunku zdobycia wartosci wyzszej od terazniejszego maksymuma (na razie nie uzywany)
 * \param pointCounter Zmienna wskazujaca na licznik punktow dla jego podalszego zwiekszenia (na razie nie uzywany)
 */
void moveGridLeft(Cell grid[GRID_SIZE][GRID_SIZE],
	int* maxNumber,
	int* pointCounter)
{
	moveGrid(grid, LEFT, maxNumber, pointCounter);
}
/**
 * @brief Funkcja dla przesuniecia komorek w gore.
 * @details
 * Dziala w 4 kroki:
 *
 * 1 -> "otwiera" petle for ktora odpowiada za wierszy macierzy parametru Cell grid[][]
 *
 * 2 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * w petli while "przesuwa" wszystkie wartosci o jedna komorke w prawo, przypisujac elementy number
 * i color struktury Cell z komorki z indeksem cur do komorki cur + 1
 * ustawiajac number z komorki cur na 0 i color komorki cur na 255, 255, 255, czyli kolor bialy
 *
 * 3 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * jesli komorki z indeksami j i j - 1 sa jednakowe, number komorki j - 1 ustawia sie na 0,
 * a number komorki j mnozy sie na 2
 *
 * 4 -> powtorzenie kroku 2 dla usuniecia dziur pomiedzy komorkami utworzonymi po polaczeniu jednakowych komorek
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \param maxNumber Zmienna wskazujaca na licznik maksymalnej wartosci obecnej na planszy dla jego podalszej zmiany za warunku zdobycia wartosci wyzszej od terazniejszego maksymuma (na razie nie uzywany)
 * \param pointCounter Zmienna wskazujaca na licznik punktow dla jego podalszego zwiekszenia (na razie nie uzywany)
 */

void moveGridUp(Cell grid[GRID_SIZE][GRID_SIZE],
	int* maxNumber,
	int* pointCounter)
{
	moveGrid(grid, UP, maxNumber, pointCounter);
}
/**
 * @brief Funkcja dla przesuniecia komorek w dol.
 * @details
 * Dziala w 4 kroki:
 *
 * 1 -> "otwiera" petle for ktora odpowiada za wierszy macierzy parametru Cell grid[][]
 *
 * 2 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * w petli while "przesuwa" wszystkie wartosci o jedna komorke w prawo, przypisujac elementy number
 * i color struktury Cell z komorki z indeksem cur do komorki cur - 1
 * ustawiajac number z komorki cur na 0 i color komorki cur na 255, 255, 255, czyli kolor bialy
 *
 * 3 -> "otwiera" petle for ktora odpowiada za kolumny, szuka komorki z wartosciami roznymi od 0,
 * jesli komorki z indeksami j i j + 1 sa jednakowe, number komorki j + 1 ustawia sie na 0,
 * a number komorki j mnozy sie na 2
 *
 * 4 -> powtorzenie kroku 2 dla usuniecia dziur pomiedzy komorkami utworzonymi po polaczeniu jednakowych komorek
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \param maxNumber Zmienna wskazujaca na licznik maksymalnej wartosci obecnej na planszy dla jego podalszej zmiany za warunku zdobycia wartosci wyzszej od terazniejszego maksymuma (na razie nie uzywany)
 * \param pointCounter Zmienna wskazujaca na licznik punktow dla jego podalszego zwiekszenia (na razie nie uzywany)
 */
void moveGridDown(Cell grid[GRID_SIZE][GRID_SIZE],
	int* maxNumber,
	int* pointCounter)
{
	moveGrid(grid, DOWN, maxNumber, pointCounter);	
}


/**
 * @brief Funkcja wypelniajaca wszystkie dane struktur Cell.
 *
 * Chociaz nazwa mowi o "stworzeniu" planszy, naprawde funkcja poprostu zamiena wartosci na defaultowe.
 *
 * Za pomoca petli for zagniezdionej w petli for przechodzi przez kazda strukture Cell i wypelnia jej dane:
 *
 * Ustawia number na 0
 *
 * Wylicza wszystkie wspolrzedne rogow dla zgodnie z rozmiarami planszy wyznaczonej przez GRID_SIZE
 *
 * Wylicza wspolrzedne dla odrysowania wartosci number jako tekstu
 *
 * Ustawia color na 255,255,255 (bialy)
 *
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 */
void CreateGrid(Cell grid[GRID_SIZE][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			grid[i][j].number = 0;
			grid[i][j].x1 = 105 + i * 150;
			grid[i][j].y1 = 105 + j * 150;
			grid[i][j].x2 = 245 + i * 150;
			grid[i][j].y2 = 245 + j * 150;
			grid[i][j].textX = 175 + i * 150;
			grid[i][j].textY = 155 + j * 150;
			grid[i][j].color = al_map_rgb(255, 255, 255);
		}
	}
}


/**
 * @brief Funkcja szuka wolnych komurek i losowo wybiera jedna z nich, w ktorej ustawia wartosc 2.
 *
 * Przeszukuje cala plansze i zapisuje wspolrzedne wszystkich komorek z number == 0 do tablicy empty, a potem losuje liczbe od 0 do empty_counter,
 * ktora jest liczba wolnych komorek, i ustawia wartosc 2 w tej komorce
 *
 * Jesli nie ma wolnych komorek, funkcja po prostu sie konczy
 *
 * Ta funkcja jest wywolywana po kazdym ruchu, zeby dodac nowa komorke do planszy
 *
 * Ta funkcja jest tez wywolywana na poczatku gry, zeby dodac pierwsza komorke do planszy
 *
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 */
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
	grid[row][column].color = getCellColor(2);
}


/**
 * @brief Sprawdzenie czy jest mozliwosc ruchu.
 *
 * Funkcja przechodzi przez cala plansze i sprawdza czy jest jakas komorka z number == 0, jesli tak, to zwraca false, bo gra nie jest przegrana
 *
 * Jesli nie ma komorek z number == 0, funkcja sprawdza czy sa jakies dwie sasiednie komorki o tej samej wartosci, jesli tak, to tez zwraca false, bo gra nie jest przegrana
 *
 * Jesli nie ma ani wolnych komorek, ani sasiednich komorek o tej samej wartosci, funkcja zwraca true, bo gra jest przegrana
 *
 * Ta funkcja jest wywolywana po kazdym ruchu, zeby sprawdzic czy gra nie jest przegrana
 *
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \return Zwraca wartosc true typu gra jest przegrana, false jesli sa mozliwe ruchy
 */
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


/**
 * @brief Funkcja rysujaca cala plansze.
 *
 * Funkcja rysujaca cala plansze, czyli wszystkie komorki i liczby w nich zawarte, a takze licznik punktow i maksymalnej wartosci
 *
 * Dziala w 3 krokach:
 *
 * 1 -> Konwersja licznikow punktow i maksymalnej wartosci do formatu char[] dla mozliwosci wyswietlenia ich za pomoca Allegro
 *
 * 2 -> Rysowanie licznikow punktow i maksymalnej wartosci za pomoca al_draw_text
 *
 * 3 -> Przechodzenie przez cala plansze za pomoca petli for zagniezdzonej w petle for, konwersja wartosci number kazdej komorki do formatu char[] i rysowanie kazdej komorki za pomoca al_draw_filled_rounded_rectangle, a jesli number komorki jest rozny od 0, to rysowanie tej wartosci za pomoca al_draw_text
 *
 *
 * Ta funkcja jest wywolywana w kazdej klatce, zeby odrysowac cala plansze i wszystkie komorki
 *
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \param font Zmienna przechowujaca cechy tekstu do wyswietlenia
 * \param pointNumber Licznik punktow
 * \param maxNumber Maksymalna wartosc na plansze
 * \param pointCounterInChar Wartosc z licznika punktow przepisana w postaci char[] dla umozliwienia wypisania tej wartosci na ekran
 * \param maxNumberInChar Wartosc maksymuma przepisana w postaci char[] dla umozliwienia wypisania jego na ekran
 */
void DrawGrid(Cell grid[GRID_SIZE][GRID_SIZE], ALLEGRO_FONT* font, int pointNumber, int maxNumber, char pointCounterInChar[], char maxNumberInChar[]) {
	sprintf(pointCounterInChar, "%d", pointNumber);
	sprintf(maxNumberInChar, "%d", maxNumber);
	al_draw_text(font, al_map_rgb(0, 0, 0), 50, 30, ALLEGRO_ALIGN_CENTER, pointCounterInChar);
	al_draw_text(font, al_map_rgb(0, 0, 0), 50, 150, ALLEGRO_ALIGN_CENTER, maxNumberInChar);
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			sprintf(grid[i][j].numberInCharFormat, "%d", grid[i][j].number);
			al_draw_filled_rounded_rectangle(grid[i][j].x1, grid[i][j].y1, grid[i][j].x2, grid[i][j].y2, 15, 15, grid[i][j].color);
			if (grid[i][j].number != 0) al_draw_text(font, al_map_rgb(0, 0, 0), grid[i][j].textX, grid[i][j].textY, ALLEGRO_ALIGN_CENTRE, grid[i][j].numberInCharFormat);
		}
	}
}

/**
 * @brief Funkcja dla restartu gry.
 *
 * Funkcja dla restartu gry, wywolywana przy nacisnieciu na R
 *
 * Dziala w 3 krokach:
 *
 * 1 -> Wywolanie funkcji CreateGrid dla ustawienia wszystkich wartosci struktur Cell na defaultowe
 *
 * 2 -> Wywolanie funkcji CalculateAndFillRandomCell dla dodania pierwszej komorki do planszy
 *
 * 3 -> Ustawienie wszystkich zmiennych odpowiedzialnych za licznik punktow, maksymalna wartosc, stan gry, mozliwosc cofania, itd na defaultowe
 *
 * Ta funkcja jest wywolywana przy nacisnieciu na R, zeby zrestartowac gre
 *
 * \param grid Odnosi sie do macierzy struktur typu Cell; nadaje dostep do wszystkich potrzebnych danych dla "przesuniecia" komorek za pomoca przechowania struktur w tabele dwuwymiarowej, na danych ktorej potem wykonujemy operacje
 * \param pointCounter Licznik punktow
 * \param maxNumber Maksymalna wartosc na plansze
 * \param gameOver True jesli gra przegrana, false jesli nie
 * \param gameWon True jesli gra wygrana, false jesli nie
 * \param alreadyWon Sprawdzenie czy gra jest wygrana
 * \param canUndo Sprawdzenie czy jest mozliwosc cofniecia ruchu
 * \param previousPointCounter Licznik punktow do ostatniego ruchu
 * \param previousMaxNumber Maksymum na plansze do ostatniego ruchu
 */
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
	ALLEGRO_FONT* font = al_load_ttf_font("Montserrat-Bold.ttf", 40, 0);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	ALLEGRO_SAMPLE* click_sound = al_load_sample("sound-effect-499.mp3");
	ALLEGRO_BITMAP* background_img = al_load_bitmap("Frame 1.jpg");


	ALLEGRO_EVENT event;
	int redrawFrame = 1;

	int pointCounter = 0;
	int maxNumber = 0;

	int highScore = 0;       
	int moveCounter = 0;    
	time_t startTime = time(NULL);

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
	moveType = NULL;

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
						moveCounter++;
						pointCounter = 0;
						for (int i = 0; i < GRID_SIZE; i++) {
							for (int j = 0; j < GRID_SIZE; j++) {
								pointCounter += grid[i][j].number;
								if (maxNumber < grid[i][j].number) maxNumber = grid[i][j].number;
							}
						}
						if (pointCounter > highScore) highScore = pointCounter;
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
				char buffer[128];
				int gameTime = (int)(time(NULL) - startTime);

				al_draw_filled_rectangle(0, 0, size, size, al_map_rgba(0, 0, 0, 210));

				al_draw_text(font, al_map_rgb(255, 50, 50), size / 2, size / 2 - 200,
					ALLEGRO_ALIGN_CENTER, "PRZEGRANA!");

				sprintf(buffer, "Wynik: %d", pointCounter);
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2 - 120,
					ALLEGRO_ALIGN_CENTER, buffer);

				sprintf(buffer, "Najwiekszy: %d", maxNumber);
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2 - 60,
					ALLEGRO_ALIGN_CENTER, buffer);

				sprintf(buffer, "Ruchy: %d", moveCounter);
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2,
					ALLEGRO_ALIGN_CENTER, buffer);

				sprintf(buffer, "Czas: %d s", gameTime);
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2 + 60,
					ALLEGRO_ALIGN_CENTER, buffer);

				sprintf(buffer, "Rekord: %d", highScore);
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2 + 120,
					ALLEGRO_ALIGN_CENTER, buffer);

				sprintf(buffer, "Nacisnij R zeby zaczaz od nowa!");
				al_draw_text(font, al_map_rgb(255, 255, 255), size / 2, size / 2 + 180,
					ALLEGRO_ALIGN_CENTER, buffer);
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