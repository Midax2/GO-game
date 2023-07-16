#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include<stdlib.h>
#include<windows.h>
#include<string.h>
#define START_X 55 // punkt X początkowy dla planszy
#define START_Y 3 // punkt Y początkowy dla planszy
#define PRZESUN_X 4 // eskalacja planszy w poziomie
#define PRZESUN_Y 2 // eskalacja planszy w pionie
#define GRANICA_PLANSZY_X PRZESUN_X * gra->rozmiar_planszy + PRZESUN_X // rozmiar planszy wyrysowanej w konsoli w poziomie
#define GRANICA_PLANSZY_Y PRZESUN_Y * gra->rozmiar_planszy + PRZESUN_Y // rozmiar planszy wyrysowanej w konsoli w pionie
#define GRANICA_PLANSZY_X_MAX PRZESUN_X * 13 + PRZESUN_X // rozmiar planszy wyrysowanej w konsoli w poziomie
#define GRANICA_PLANSZY_Y_MAX PRZESUN_Y * 13 + PRZESUN_Y // rozmiar planszy wyrysowanej w konsoli w pionie
#define KONIEC_X START_X + gra->granica_x // punkt X końcowy dla planszy
#define KONIEC_Y START_Y + gra->granica_y // punkt Y końcowy dla planszy
#define GORA 0x48 // przycisk strzałki w górę
#define DOL 0x50 // przycisk strzałki w dół
#define LEWO 0x4b // przycisk strzałki w lewo
#define PRAWO 0x4d // przycisk strzałki w prawo
#define SPACE 0x20 // przycisk spacji
#define ENTER 0x0d // przycisk enter
#define PUSTE 0x2b //oznakowanie pustego miejsca w planszy
#define CZARNE 0x42 //oznakowanie miejsca w planszy zajętego przez czarny kamień
#define BIALE 0x57 //oznakowanie miejsca w planszy zajętego przez biały kamień
#define POSTAW_KAMIEN 0x69 // przycisk "i", inaczej przycisk do postawienia kamienia na planszy
#define RESET_PLANSZY 0x6e // przycisk "n", inaczej przycisk do zresetowania planszy
#define SAVE 0x73 // przycisk do zapisywania gry
#define LOAD 0x6c // przycisk do odczytywania gry
#define BACKSPACE 0x08 // przycisk backspace'u. Usuwa ostatni znak z lewej
#define HANDICAP 0x68 // przycisk kończoncy ustawienie czarnych kamieni przy grze z handicap'em
#define ESC 0x1b // przycisk escape'u // do kasowania wyboru ruchu
#define ILE_INSTRUKCJI 17 // ilość podanych instrucji
#define MAX_DL_INSTRUKCJI 100 // maksymalna długość instrukji w znakach
#define MENU_X 3 // punkt X początkowy dla menu
#define MENU_Y 3 // punkt Y początkowy dla menu
#define PIERWSZY_GRACZ CZARNYCH // ustawienie pierwszego gracza w grze
#define DL_SAVE_NAME 32 // maksymalna długość nazwy save pliku
#define ILOSC_GRACZY 2 // ilość graczy uczęsniczących w grze
#define NIEUZYWANE_POLA -3 // puste miejsca w tablice
#define SCIANY -4 // ściany planszy
#define ODL_PUNKTAMI 12 // odległość pomiędzy punktami graczy przy wypisaniu na ekran
#define ZERO_PKT 0 // punkt początkowy wyświetlanych punktów graczy
#define START_MALEJ_PLANSZY 1 // początkowy, używany indek tabeli pomocniczej
#define KAMIEN_DOOKOL 4 // kamień jest uznany za otoczony przy tej wartości
#define ILE_POZYCJI_TAB_ROZMIAR_PLANSZY 6 // ilość pozycji w tabeli z instrukcjami do wyboru rozmiaru planszy
#define ILE_INSTRUKCJI_TAB_ROZMIAR_PLANSZY ILE_POZYCJI_TAB_ROZMIAR_PLANSZY - 2 // ile rozmiarów jest na wybór
#define PRZESUWANIE_LICZBY 10 // przy wpisywaniu rozmiaru liczba pomocnicza jest 
// dzielona i mnożona przez 10, by uzyskać poprawny resultat w systemie dziesiętnym
#define DEFAULT_ROZMIAR 13 // domyślny rozmiar planszy
#define CZAS_OCZYKIWANIA 4000 // czas, podczas którego jest wyświetlana informacja
#define TEMP_PLANSZA gra->runda + 2 // określenie plików do tymczasowego przechowywania planszy
#define TERAZ_PLANSZA 4 // nazwa pliku do przechowywania obecnej planszy
#define ILE_PLIKOW 5 // ilość plików do przechowywania plansz
// współrzędne wypisania instrukcji do handicap'u
#define HANDICAP_X MENU_X + 15
#define HANDICAP_Y MENU_Y + 2

enum czyja_runda {
	CZARNYCH, BIALYCH // definicja graczy
};

enum sprawdzanie_warunku {
	NIE, TAK // definicja warunków (odpowiedniki true i false)
};
enum tryb_zapisu {
	PODSTAWOWY, TEMP, TERAZ // tryby do zapisu plansz do plików
};
enum czy_sprawdzone {
	NIE_SPRAWDZONE, SPRAWDZONE // czy podane współrzędne już zostałe sprawdzone podczas sxukania łańcuchów
};

typedef struct {
	int punkty;
}gracze_t; // struktura elementów poszczególnych graczy

typedef struct {
	char** plansza_mala; // plansza pomocnicza, która jest używana do wszelkich obliczeń
	bool** sprawdzone; // przechowywane tu są sprawdzone punkty podczas szukania łańcuchów
}plansze_t; // struktura potrzebnych do działania gry tablic do plansz

typedef struct {
	int x; // obecny punkt x kursoru
	int y; // obecny punkt y kursoru
}wspolrz_t;

typedef struct {
	// współrzędne kamieni będących łańcuchem
	int* cor_x;
	int* cor_y;
	// ilość kamieni
	int kamieni;
}wsp_lancuchy_t;

typedef struct {
	plansze_t plansze;
	gracze_t gracz[ILOSC_GRACZY];
	char* punkty_char; // przechowuje napis do wyświetlenia
	int runda;
	int rozmiar_planszy;
	// pozycje kursora
	wspolrz_t wsp;
	// granice wyrosywania się w konsoli planszy do gry
	int granica_x;
	int granica_y;
	// punkty początkowe planszy do wyrysowania się w konsoli
	int rysowanie_start_x;
	int rysowanie_start_y;
	// sprawzenie, czy gracz w swojej rundzie już umieścił kamień
	bool juz_stoi;
	// sprawzenie, czy aktywowany jest handicap
	bool nie_handicap;
}gra_t; // struktura elementów całej gry

/* Comment: in the final program declare appropriate constants, e.g.,
   to eliminate from your program numerical values by replacing them
   with well defined identifiers */

// plansze dynamicznie uzyskują rozmiar podany przez gracza
void tworz_tablice(gra_t *gra) {
	gra->plansze.plansza_mala = (char**)malloc((gra->rozmiar_planszy + 2) * sizeof(char*));
	for (int i = 0; i < gra->rozmiar_planszy + 2; i++) {
		gra->plansze.plansza_mala[i] = (char*)malloc((gra->rozmiar_planszy + 3) * sizeof(char));
	}
	gra->plansze.sprawdzone = (bool**)malloc((gra->rozmiar_planszy + 3) * sizeof(bool*));
	for (int i = 0; i < gra->rozmiar_planszy + 3; i++) {
		gra->plansze.sprawdzone[i] = (bool*)malloc((gra->rozmiar_planszy + 3) * sizeof(bool));
	}
}

// przydzielanie pamięci tablicom przechowującym współrzędne grup kamieni
void tworz_tab_wsp(wsp_lancuchy_t* wsp_lan, int rozmiar) {
	wsp_lan->cor_x = (int*)malloc((rozmiar * rozmiar) * sizeof(int));
	wsp_lan->cor_y = (int*)malloc((rozmiar * rozmiar) * sizeof(int));
}
// zwalnianie pamięci użytej do nadania rozmiaru planszom i tablicam
void zwalnianie_pamieci(gra_t *gra) {
	for (int i = 0; i < gra->rozmiar_planszy + 2; i++) {
		free(gra->plansze.plansza_mala[i]);
	}
	free(gra->plansze.plansza_mala);
	for (int i = 0; i < gra->rozmiar_planszy + 3; i++) {
		free(gra->plansze.sprawdzone[i]);
	}
	free(gra->plansze.sprawdzone);
}

// funkcja zamieniająca wartość dziesiętną na tablicę znaków
void int_to_char_array(gra_t *gra, int liczba) {
	int temp = liczba;
	int dl = 0;
	// wyznaczanie długości liczby
	while (temp > 0) {
		dl++;
		temp /= 10;
	}
	// przydzielanie pamięci na tablicę znaków
	gra->punkty_char = (char*) calloc(dl+1, sizeof(char));
	// przpisywanie wartości do tablicy
	for (int i = dl-1; i >= 0; --i, liczba /= 10) {
			gra->punkty_char[i] = (liczba % 10) + '0';
	}
}

// funkcja sprawdzająca, czy podany plik istnieje
bool czy_plik_istnieje(const char file[]) {
	FILE* plik;
	if (plik = fopen(file, "r")) {
		fclose(plik);
		return TAK;
	}
	return NIE;
}

// sprawdzanie, czy podane dwa pliki są identyczne
bool porownanie_plikow(FILE* file_1, FILE* file_2) {
	char znak_1, znak_2;
	// porównywanie przebiega znak po znaku
	while (((znak_1 = fgetc(file_1)) != EOF) && ((znak_2 = fgetc(file_2)) != EOF)) {
		if (znak_1 == znak_2) {
			continue;
		}
		else {
			return NIE;
		}
	}
	return TAK;
}

// komentarz do wszystkich funkcji save/load: dynamiczne tablice nie zapisują/odczytują się razem z całą strukturą
// więc zapisywany/odczytywany jest każdy element osobno

// funkcja zapisująca binarnie planszę do pliku w trybie zależnym od parameru
void zapisz_plansza(gra_t *gra, int tryb_zapisu) {
	char name[ILOSC_GRACZY];
	if (tryb_zapisu == PODSTAWOWY)
		_itoa(gra->runda, name, 10);
	else if (tryb_zapisu == TEMP)
		_itoa(TEMP_PLANSZA, name, 10);
	else
		_itoa(TERAZ_PLANSZA, name, 10);
	FILE* save = fopen(name, "w+");
	for (int i = 0; i < gra->rozmiar_planszy + 2; i++) {
		for (int j = 0; j < gra->rozmiar_planszy + 2; j++) {
			fwrite(&gra->plansze.plansza_mala[i][j], sizeof(char), 1, save);
		}
	}
	fclose(save);
}
// funkcja odczytująca binarnie planszę z pliku w trybie zależnym od parameru
void odczytaj_save_plansza(gra_t* gra, int tryb_zapisu) {
	char name[ILOSC_GRACZY];
	if (tryb_zapisu == PODSTAWOWY)
		_itoa(gra->runda, name, 10);
	else if (tryb_zapisu == TEMP)
		_itoa(TEMP_PLANSZA, name, 10);
	else
		_itoa(TERAZ_PLANSZA, name, 10);
	FILE* load = fopen(name, "r+");
	for (int i = 0; i <= gra->rozmiar_planszy + 1; i++) {
		for (int j = 0; j <= gra->rozmiar_planszy + 1; j++) {
			fread(&gra->plansze.plansza_mala[i][j], sizeof(char), 1, load);
		}
	}
	fclose(load);
}

// ustawia nazwe pliku do zapisu/odczytu gry na podany przez użytkownika
void nazwa_pliku(char* name) {
	clrscr();
	int zn = 0;
	int ile = 0;
	while (zn != ENTER) {
		gotoxy(MENU_X, MENU_Y);
		cputs("Enter the name of save file (max 32 character): ");
		gotoxy(MENU_X + 50 + ile, MENU_Y);
		zn = getche();
		if ((zn >= 'A' && zn <= 'Z') || (zn >= 'a' && zn <= 'z')) {
			name[ile] = zn;
			ile++;
		}
		if (zn == BACKSPACE) {
			clreol();
			ile--;
		}
	}
	clrscr();
	if (ile <= 0 || ile >= DL_SAVE_NAME) {
		cputs("File name is invalid. Name is set to (Default)");
		Sleep(CZAS_OCZYKIWANIA);
		char temp[] = "Default";
		strcpy(name, temp);
	}
}

// funkcja zapisująca binarnie do pliku całą gre
void zapisz_gra(gra_t *gra) {
	char name[DL_SAVE_NAME];
	nazwa_pliku(name);
	FILE* save = fopen(name, "w+");
	fwrite(&gra, sizeof(gra_t), 1, save);
	for (int i = 0; i <= gra->rozmiar_planszy + 1; i++) {
		for (int j = 0; j <= gra->rozmiar_planszy + 1; j++) {
			fwrite(&gra->plansze.plansza_mala[i][j], sizeof(char), 1, save);
		}
	}
	fclose(save);
}
// funkcja odczytująca binarnie z pliku całą gre
void odczytaj_save_gra(gra_t* gra) {
	char name[DL_SAVE_NAME];
	nazwa_pliku(name);
	if (czy_plik_istnieje(name)) {
		FILE* load = fopen(name, "r+");
		fread(&gra, sizeof(gra_t), 1, load);
		for (int i = 0; i <= gra->rozmiar_planszy + 1; i++) {
			for (int j = 0; j <= gra->rozmiar_planszy + 1; j++) {
				fread(&gra->plansze.plansza_mala[i][j], sizeof(char), 1, load);
			}
		}
		fclose(load);
	}
}

// sprawdza, czy podany kamień był już dodany do grupy
bool czy_juz_spr(wsp_lancuchy_t* wsp_lan, int x, int y) {
	for (int i = 0; i < wsp_lan->kamieni; i++) {
		if (wsp_lan->cor_x[i] == x && wsp_lan->cor_y[i] == y)
			return TAK;
	}
	return NIE;
}

// rekurencyjne poszukiwanie grup kamieni, będących w bezpośrednim kontakcie ze sobą
void szukaj_lancuch(gra_t *gra, wsp_lancuchy_t *wsp_lan, int x, int y, char ja) {
	wsp_lan->cor_x[wsp_lan->kamieni] = x;
	wsp_lan->cor_y[wsp_lan->kamieni] = y;
	wsp_lan->kamieni++;
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 1; j <= x + 1; j++) {
			// sprawdzane są kamieni z przodu, tyłu, z lewa i z prawa
			if (i == y && j == x)
				continue;
			if (i == y - 1 && (j == x - 1 || j == x + 1))
				continue;
			if (i == y + 1 && (j == x - 1 || j == x + 1))
				continue;
			// wszystkie do tej pory sprawdzone kamienie na całej planszy są pomijane
			if (gra->plansze.plansza_mala[i][j] == SPRAWDZONE)
				continue;
			// oraz są pomijane kamienie już dodane w tym cyklu do grupy
			if (czy_juz_spr(wsp_lan, j, i))
				continue;
			// jeżeli pozostały jeszcze takie kamienie, to szukaj dalej
			if (gra->plansze.plansza_mala[i][j] == ja) {
				szukaj_lancuch(gra, wsp_lan, j, i, ja);
			}
		}
	}
}

void stworz_plansza (gra_t *gra) {
	// ustawianie wszystkich elementów planszy na puste
	for (int i = 0; i <= gra->rozmiar_planszy + 1; i++) {
		for (int j = 0; j <= gra->rozmiar_planszy + 1; j++) {
			if (i == 0 || i == gra->rozmiar_planszy + 1 || j == 0 || j == gra->rozmiar_planszy + 1)
				gra->plansze.plansza_mala[i][j] = SCIANY;
			else
				gra->plansze.plansza_mala[i][j] = PUSTE;
		}
	}
	// ustawianie początkowych wspórzędnych kursora na planszy
	gra->wsp.x = START_X + PRZESUN_X;
	gra->wsp.y = START_Y + PRZESUN_Y;
	// wyznaczanie granicy wyrysowania się planszy w konsoli w zależności od podanego rozmiaru
	if (gra->rozmiar_planszy > DEFAULT_ROZMIAR) {
		gra->granica_x = GRANICA_PLANSZY_X_MAX;
		gra->granica_y = GRANICA_PLANSZY_Y_MAX;
	}
	else {
		gra->granica_x = GRANICA_PLANSZY_X;
		gra->granica_y = GRANICA_PLANSZY_Y;
	}
	// początkowe punkty rysowania planszy w konsoli
	gra->rysowanie_start_x = 1;
	gra->rysowanie_start_y = 0;
}

void rysuj_plansza(gra_t *gra) {
	for (int i = 0, i_m = gra->rysowanie_start_y; i <= gra->granica_y; i++) { // wyrysowanie planszy do gry
		if (i % PRZESUN_Y == 0 && i > 0 && i < gra->granica_y)
			i_m++;
		for (int j = 0, j_m = gra->rysowanie_start_x; j <= gra->granica_x; j++) {
			gotoxy(START_X + j, START_Y + i); // miejsce wyrysowania elementu planszy
			if (j % PRZESUN_X == 0 && i % PRZESUN_Y == 0 && i > 0 && i < gra->granica_y && j > 0 && j < gra->granica_x) {
				putch(gra->plansze.plansza_mala[i_m][j_m]); // elementy styczności, na których toczy się gr
				if (i_m == gra->rysowanie_start_y + 1) {
					gotoxy(START_X + j, START_Y + i - PRZESUN_Y - 1);
					int_to_char_array(gra, j_m);
					cputs(gra->punkty_char); // pozycja według osi X
					free(gra->punkty_char);
				}
				if (j_m == gra->rysowanie_start_x) {
					gotoxy(START_X + j - PRZESUN_X - 2, START_Y + i);
					int_to_char_array(gra, i_m);
					cputs(gra->punkty_char); // pozycja według osi Y
					free(gra->punkty_char);
				}
				j_m++;
			}
			else if (i > 0 && i < gra->granica_y && j >= 0 && j <= gra->granica_x && j % PRZESUN_X == 0)
				putch('|'); // stworzenie pionowych elementów wewnątrz planszy
			else if (i >= 0 && i <= gra->granica_y && j > 0 && j < gra->granica_x && i % PRZESUN_Y == 0)
				putch('-'); // stworzenie poziomych elementów wewnątrz planszy
		}
	}
}

void rysuj_kursor(gra_t *gra) {
	// we draw a star
	gotoxy(gra->wsp.x, gra->wsp.y);
	// putch prints one character and moves the cursor to the right
	putch('*');
}

void zeruj_punkty(gra_t *gra) {
	// zerowanie punktów obydwóm graczom
	for (int i = 0; i < ILOSC_GRACZY; i++) {
		gra->gracz[i].punkty = 0;
	}
	gra->juz_stoi = NIE;
}

void pisz_menu() {
	// we set black to be the background color
		// check conio2.h for available colors
	textbackground(BLACK);
	// clear the screen: we fill it out with spaces with
	// the specified background color
	clrscr();
	// we set the text color
	textcolor(LIGHTGRAY);

	// spis instrukcji
	char instrukcje[ILE_INSTRUKCJI][MAX_DL_INSTRUKCJI] = {
		"autor   = Dmytro Dzhusov, 196751",
		"",
		"q       = exit",
		"arrows  = moving the cursor over the board",
		"n       = start a new game",
		"enter   = confirm choice and end player’s turn",
		"esc     = cancel current action",
		"i       = place a stone on the board",
		"s       = save the game state",
		"l       = load the game state",
		"f       = finish the game",
		"h       = stop game state editing",
		"",
		"Black:      White:",
		"",
		"",
		"Turn:"
	}; 

	// wypisywanie instrukcji strerowania
	for (int i = 0; i < ILE_INSTRUKCJI; i++) {
		gotoxy(MENU_X, MENU_Y + i);
		cputs(instrukcje[i]);
	}
}

// zamienianie podanego elemetu planszy na kamień czarny
void zmien_na_czarne(gra_t* gra, int teraz_y, int teraz_x) {
	gra->plansze.plansza_mala[teraz_y][teraz_x] = CZARNE;
}

// zamienianie podanego elemetu planszy na kamień biały
void zmien_na_biale(gra_t* gra, int teraz_y, int teraz_x) {
	gra->plansze.plansza_mala[teraz_y][teraz_x] = BIALE;
}

// zamienianie podanego elemetuplanszy na miejszce puste
void zmien_na_puste(gra_t* gra, int teraz_y, int teraz_x) {
	gra->plansze.plansza_mala[teraz_y][teraz_x] = PUSTE;
	gra->juz_stoi = NIE;
}

// wypisywanie punktów graczy w konsoli
void pisz_punkty(gra_t *gra) {
	for (int i = 0; i < ILOSC_GRACZY; i++) {
		gotoxy(MENU_X + i * ODL_PUNKTAMI, MENU_Y + ILE_INSTRUKCJI - 3);
		int temp = gra->gracz[i].punkty;
		int_to_char_array(gra, temp);
		if (temp == ZERO_PKT)
			putch('0');
		else
			cputs(gra->punkty_char);
		free(gra->punkty_char);
	}
}

// wypisywanie, którego graczy jest runda w konsoli
void pisz_runda(gra_t* gra) {
	gotoxy(MENU_X, MENU_Y + ILE_INSTRUKCJI);
	if (gra->runda == CZARNYCH)
		cputs("Black");
	else
		cputs("White");
}

// funkcja, ustalająca wroga gracza obecnej rundy
char kto_ja(gra_t *gra) {
	if (gra->runda == CZARNYCH)
		return CZARNE;
	else
		return BIALE;
}

// wybieranie rozmiaru planszy przez gracza
void wybieranie_rozmiaru(gra_t *gra, int *zn) {
	// instrukcje do wybierania rozmiaru
	char rozmiary_plansz_string[ILE_POZYCJI_TAB_ROZMIAR_PLANSZY][MAX_DL_INSTRUKCJI] = {
		"Board size 5x5: ",
		"Board size 13x13: ",
		"Board size 19x19: ",
		"Your own size: : ",
		"",
		"Press ENTER to confirm"
	};
	// moźliwości wyboru
	int rozmiary_plansz_int[ILE_INSTRUKCJI_TAB_ROZMIAR_PLANSZY]{
		5, 13, 19, NIEUZYWANE_POLA
	};
	// ustawianie początkowej pozycji kursora podczas wybierania rozmiaru planszy
	gra->wsp.x = MENU_X + 30;
	gra->wsp.y = MENU_Y;
	while (*zn != ENTER) {
		clrscr();
		for (int i = 0; i < ILE_POZYCJI_TAB_ROZMIAR_PLANSZY; i++) {
			// wypisanie instrukcji
			gotoxy(MENU_X, MENU_Y + i);
			cputs(rozmiary_plansz_string[i]);
		}
		rysuj_kursor(gra);
		*zn = getch();
		// wybieranie rozniaru
		if (*zn == GORA && gra->wsp.y > MENU_Y)
			gra->wsp.y--;
		if (*zn == DOL && gra->wsp.y < MENU_Y + ILE_INSTRUKCJI_TAB_ROZMIAR_PLANSZY - 1)
			gra->wsp.y++;
		if (*zn == ENTER) {
			for (int i = 0; i < ILE_INSTRUKCJI_TAB_ROZMIAR_PLANSZY; i++) {
				if (wherey() - MENU_Y == i) {
					gra->rozmiar_planszy = rozmiary_plansz_int[i];
				}
			}
			break;
		}
	}
	clrscr();
	// wprowadzenie przez gracza własnego rozmiaru
	if (gra->rozmiar_planszy == NIEUZYWANE_POLA) {
		int temp = 0;
		*zn = 0;
		gotoxy(MENU_X, MENU_Y);
		cputs("Enter the board size: ");
		// korzystając z własności liczb w systemie dziesiętnym jest wprowadzany rozmiar planszy
		while (*zn != ENTER) {
			*zn = getche();
			if (*zn == ENTER) {
				// zapisanie wyniku
				temp /= PRZESUWANIE_LICZBY;
				break;
			}
			else if (*zn == BACKSPACE) {
				// imitacja usuwania ostatniej podanej cyfry
				temp /= (PRZESUWANIE_LICZBY * PRZESUWANIE_LICZBY);
				clreol();
			}
			else
				// dodanie wprowadzonej cyfry z tyłu liczby
				temp += *zn - '0';
			temp *= PRZESUWANIE_LICZBY;
		}
		if (temp > 0)
			// ustawienie wyniku wprowadzanego rozmiaru na rozmiar planszy
			gra->rozmiar_planszy = temp;
		else {
			// jeżeli wprowadzony rozmiar jest mniejszy od zera, czyli nie poprawny
			// to rozmiar planszy jest ustawiany na domyślny, czyli 9
			// by wprowadzić ponownie trzeba zacząć nową grę
			cputs("Board size is incorrect. Size is set to default value. Default = 13. To choose again, please, restart the game.");
			gra->rozmiar_planszy = DEFAULT_ROZMIAR;
			Sleep(CZAS_OCZYKIWANIA);
		}
	}
}

// gra z handicap'em, czyli wprowadzenie przez gracza kamieni czarnych początkowej konfiguracji
void handicap(gra_t* gra) {
	// instrukcje do handicap'u
	char instrukcje_do_handicap[][MAX_DL_INSTRUKCJI] = {
		"Do you want to play with handicap? (placing black stones before starting the game)",
		"",
		"Yes",
		"No",
		"",
		"Press ENTER to confirm"
	};
	int rozmiar_tabeli = sizeof(instrukcje_do_handicap) / sizeof(instrukcje_do_handicap[0]);
	gra->wsp.x = HANDICAP_X;
	gra->wsp.y = HANDICAP_Y;
	int zn = 0;
	while (zn != ENTER) {
		clrscr();
		// wypisanie instrukcji
		for (int i = 0; i < rozmiar_tabeli; i++) {
			gotoxy(MENU_X, MENU_Y + i);
			cputs(instrukcje_do_handicap[i]);
		}
		rysuj_kursor(gra);
		// wybieranie opcji
		zn = getch();
		if (zn == GORA && gra->wsp.y > MENU_Y + 2)
			gra->wsp.y--;
		if (zn == DOL && gra->wsp.y < MENU_Y + rozmiar_tabeli - 3)
			gra->wsp.y++;
	}
	gra->nie_handicap = gra->wsp.y - MENU_Y - 2;
}

void przemieszczenie_kursoru(gra_t *gra, int *zn) {
	*zn = getch();
	// przesuwanie kursoru z ograniczeniem w postaci granic planszy
	// jeżeli rozmiar planszy jest większy od maksymalnego do wyrysowania, to przy osiąganiu granicy wyrysowania
	// to widoczna plansza jest przesuwana w stronę, gdzie idzie kursor
	if (*zn == GORA) {
		if (wherey() - PRZESUN_Y > START_Y)
			gra->wsp.y -= PRZESUN_Y;
		if (wherey() - (2 * PRZESUN_Y) < START_Y && gra->rysowanie_start_y > 0)
			gra->rysowanie_start_y--;
	}
	else if (*zn == DOL) {
		if (wherey() + PRZESUN_Y < KONIEC_Y)
			gra->wsp.y += PRZESUN_Y;
		if (wherey() + (2 * PRZESUN_Y) > KONIEC_Y && gra->rysowanie_start_y < gra->rozmiar_planszy - DEFAULT_ROZMIAR)
			gra->rysowanie_start_y++;
	}
	else if (*zn == LEWO) {
		if (wherex() - PRZESUN_X > START_X + 1)
			gra->wsp.x -= PRZESUN_X;
		if (wherex() - (2 * PRZESUN_X) < START_X + 1 && gra->rysowanie_start_x > 1)
			gra->rysowanie_start_x--;
	}
	else if (*zn == PRAWO) {
		if (wherex() + PRZESUN_X < KONIEC_X)
			gra->wsp.x += PRZESUN_X;
		if (wherex() + PRZESUN_X > KONIEC_X && gra->rysowanie_start_x <= gra->rozmiar_planszy - DEFAULT_ROZMIAR)
			gra->rysowanie_start_x++;
	}
}

// wyczyszczenie tablicy z współrzędnymi elementów łańcuchu
void czyszc_tab(wsp_lancuchy_t *wsp_lan, int rozmiar) {
	for (int i = 0; i < rozmiar * rozmiar; i++) {
		wsp_lan->cor_x[i] = NIEUZYWANE_POLA;
		wsp_lan->cor_y[i] = NIEUZYWANE_POLA;
	}
}

// sprawdzenie, czy grupa kamieni ma jeszcze wolne oddechy
bool wolne_oddechy(wsp_lancuchy_t* wsp_lan, gra_t *gra) {
	for (int q = 0; q < wsp_lan->kamieni; q++) {
		for (int i = (int)wsp_lan->cor_y[q] - 1; i <= (int)wsp_lan->cor_y[q] + 1; i++) {
			for (int j = (int)wsp_lan->cor_x[q] - 1; j <= (int)wsp_lan->cor_x[q] + 1; j++) {
				if (i == wsp_lan->cor_y[q] && j == wsp_lan->cor_x[q])
					continue;
				if (i == wsp_lan->cor_y[q] - 1 && (j == wsp_lan->cor_x[q] - 1 || j == wsp_lan->cor_x[q] + 1))
					continue;
				if (i == wsp_lan->cor_y[q] + 1 && (j == wsp_lan->cor_x[q] - 1 || j == wsp_lan->cor_x[q] + 1))
					continue;
				if (gra->plansze.plansza_mala[i][j] == PUSTE)
					return TAK;
			}
		}
	}
	return NIE;
}

// jeżeli grupa kamieni nie ma wolnych oddechów, to dodawane są punkty za zbite kamienie do przeciwnika i grupa kamieni znika z planszy
void dodaj_pkt(wsp_lancuchy_t* wsp_lan, gra_t* gra) {
	gra->gracz[gra->runda].punkty += wsp_lan->kamieni;
	for (int i = 0; i < wsp_lan->kamieni; i++) {
		gra->plansze.plansza_mala[(int)wsp_lan->cor_y[i]][(int)wsp_lan->cor_x[i]] = PUSTE;
	}
}

// usuwanie plików z planszami  
void usuwanie_plikow(gra_t* gra) {
	for (int i = 0; i < ILE_PLIKOW; i++) {
		int_to_char_array(gra, i);
		remove(gra->punkty_char);
		free(gra->punkty_char);
	}
}

// wykrywanie łańcuchów na planszy i sprawdzanie, czy mają one jeszcze wolne oddechy
void wszystkie_lancuchy(gra_t* gra) {
	wsp_lancuchy_t wsp_lan;
	tworz_tab_wsp(&wsp_lan, gra->rozmiar_planszy);
	// wszystkie elementy planszy są niesprawdzone
	for (int i = 1; i <= gra->rozmiar_planszy; i++) {
		for (int j = 1; j <= gra->rozmiar_planszy; j++) {
			gra->plansze.sprawdzone[i][j] = NIE_SPRAWDZONE;
		}
	}
	// poszukiwanie wszystkich grup kamieni na plansze i sprawdzanie ich oddechów
	// w razie zbicia kamieni, zwiekszane są punkty obecnego gracza
	for (int i = 1; i <= gra->rozmiar_planszy; i++) {
		for (int j = 1; j <= gra->rozmiar_planszy; j++) {
			czyszc_tab(&wsp_lan, gra->rozmiar_planszy);
			wsp_lan.kamieni = 0;
			if (gra->runda == CZARNYCH && gra->plansze.plansza_mala[i][j] == BIALE) {
				szukaj_lancuch(gra, &wsp_lan, j, i, BIALE);
			}
			else if (gra->runda == BIALYCH && gra->plansze.plansza_mala[i][j] == CZARNE) {
				szukaj_lancuch(gra, &wsp_lan, j, i, CZARNE);
			}
			if (!wolne_oddechy(&wsp_lan, gra))
				dodaj_pkt(&wsp_lan, gra);
			for (int i = 0; i < wsp_lan.kamieni; i++) {
				gra->plansze.sprawdzone[wsp_lan.cor_y[i]][wsp_lan.cor_x[i]] = SPRAWDZONE;
			}
		}
	}
	free(wsp_lan.cor_x);
	free(wsp_lan.cor_y);
}

// zmienianie rundy na kolejnego gracza
void zmien_gracza(gra_t* gra) {
	(gra->runda)++;
	gra->runda %= ILOSC_GRACZY;
}

// sterowanie grą
void sterowanie(int *zn, gra_t *gra) {
	*zn = getch();
	int teraz_y = ((gra->wsp.y - START_Y)/ PRZESUN_Y) + gra->rysowanie_start_y, teraz_x = ((gra->wsp.x - START_X) / PRZESUN_X) + gra->rysowanie_start_x - 1;
	char name[ILOSC_GRACZY];
	_itoa(gra->runda, name, 10);
	// we do not want the key 'H' to play role of "up arrow"
	// so we check if the first code is zero
	if (*zn == 0) {
		przemieszczenie_kursoru(gra, zn);
	}
	// ustawienie kamienia na planszy
	else if (*zn == POSTAW_KAMIEN && gra->plansze.plansza_mala[teraz_y][teraz_x] == PUSTE) {
		// jeżeli plik z planszą przed ruchiem poprzedniego gracza jest taki sam co plansza po wstawieniu
		// kamienia przez obecnego gracza, to zaszła reguła Ko
		// plansza jest powracana do stanu, w jakim była przed wstawieniem kamienia
		// przez to, że usuwane są kamienie i zmieniane w procesie punkty, to przed tym jest tworzony backup
		// i później użyty do przywracania stanu pubktów
		int temp_pkt_1 = gra->gracz[CZARNYCH].punkty;
		int temp_pkt_2 = gra->gracz[BIALYCH].punkty;
		// zabiezpieczenie od wstawiania przez gracza w jednej rundzie więcej niż jednego kamienia
		if (gra->juz_stoi == NIE)
			zapisz_plansza(gra, PODSTAWOWY);
		else {
			odczytaj_save_plansza(gra, PODSTAWOWY);
			gra->juz_stoi = NIE;
		}
		// ustawienie odpowiedniego kamienia
		if (gra->runda == CZARNYCH) {
			zmien_na_czarne(gra, teraz_y, teraz_x);
		}
		else {
			zmien_na_biale(gra, teraz_y, teraz_x);
		}
		wszystkie_lancuchy(gra);
		gra->gracz[CZARNYCH].punkty = temp_pkt_1;
		gra->gracz[BIALYCH].punkty = temp_pkt_2;
		zapisz_plansza(gra, TERAZ);
		if (czy_plik_istnieje("0") && czy_plik_istnieje("1") && czy_plik_istnieje("2") && czy_plik_istnieje("3")) {
			wsp_lancuchy_t wsp_lan;
			wsp_lan.kamieni = 0;
			tworz_tab_wsp(&wsp_lan, gra->rozmiar_planszy);
			szukaj_lancuch(gra, &wsp_lan, teraz_x, teraz_y, kto_ja(gra));
			_itoa(TEMP_PLANSZA, name, 10);
			FILE* file_1, * file_2;
			file_1 = fopen(name, "r");
			_itoa(TERAZ_PLANSZA, name, 10);
			file_2 = fopen(name, "r");
			// porównanie plansz
			if (porownanie_plikow(file_1, file_2) || !wolne_oddechy(&wsp_lan, gra)) {
				free(wsp_lan.cor_x);
				free(wsp_lan.cor_y);
				odczytaj_save_plansza(gra, PODSTAWOWY);
				return;
			}
			else {
				free(wsp_lan.cor_x);
				free(wsp_lan.cor_y);
				// po sprawdzaniu reguły Ko jest odpowiednio ustawiana plansza
				odczytaj_save_plansza(gra, PODSTAWOWY);
				if (gra->runda == CZARNYCH) {
					zmien_na_czarne(gra, teraz_y, teraz_x);
				}
				else {
					zmien_na_biale(gra, teraz_y, teraz_x);
				}
			}
		}

		gra->juz_stoi = TAK;
	}
	// gra zaczyna się od początku
	else if (*zn == RESET_PLANSZY) {
		gra->runda = PIERWSZY_GRACZ;
		// wszystkie potrzebne funkcje do wyzerowania gry
		zwalnianie_pamieci(gra);
		wybieranie_rozmiaru(gra, zn);
		tworz_tablice(gra);
		handicap(gra);
		stworz_plansza(gra);
		zeruj_punkty(gra);
		usuwanie_plikow(gra);
	}
	else if (*zn == ENTER && gra->juz_stoi == TAK) {
		wszystkie_lancuchy(gra);
		zapisz_plansza(gra, TEMP);
		// ustawienie rundy na kolejnego gracza
		if (gra->nie_handicap == TAK) {
			zmien_gracza(gra);
		}
		gra->juz_stoi = NIE;
	}
	// zapisywanie stanu gry
	else if (*zn == SAVE) {
		zapisz_gra(gra);
	}
	// odczytywanie stanu gry
	else if (*zn == LOAD) {
		odczytaj_save_gra(gra);
	}
	// kończenie handicap'u i rozpoczęcie gry
	else if (*zn == HANDICAP) {
		gra->nie_handicap = TAK;
		zmien_gracza(gra);
	}
	// cofnięcie wykonanego ruchu
	else if (*zn == ESC && czy_plik_istnieje(name)) {
		odczytaj_save_plansza(gra, PODSTAWOWY);
	}
}

int main() {
	gra_t gra;
	remove("cala_gra.txt");
	usuwanie_plikow(&gra);
	zeruj_punkty(&gra);
	gra.runda = PIERWSZY_GRACZ;
	int zn = 0;
	wybieranie_rozmiaru(&gra, &zn); // wybieranie rozmiaru przez gracza
	tworz_tablice(&gra); // stworzenie plansze i przydzielenie pamięci
	// if the program is compiled in pure C, then we need to initialize
	// the library ourselves; __cplusplus is defined only if a C++ compiler
	// is used
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle sets the window title
	settitle("Project GO");
	// hide the blinking cursor
	_setcursortype(_NOCURSOR);
	handicap(&gra); // etap handicap'u w grze
	stworz_plansza(&gra); // etap stworzenia planszy

	do {
		pisz_menu(); // etap wypisywania menu w konsoli
		rysuj_plansza(&gra); // etap wyrysowania planszy w konsoli
		pisz_punkty(&gra); // wypisywanie punktów na ekran
		pisz_runda(&gra); // wypisanie rundy na ekranie
		rysuj_kursor(&gra); // etap wyrysowania kursoru w konsoli
		sterowanie(&zn, &gra); // sterowanie grą
	} while (zn != 'q');
	zwalnianie_pamieci(&gra);
	_setcursortype(_NORMALCURSOR);	// show the cursor so it will be
	// visible after the program ends
	return 0;
}