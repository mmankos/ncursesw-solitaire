#include <stdlib.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include "game.h"
#include "draw.h"

int main()
			{
				// set locale to UTF to be able to print unicode characters using ncurses
				setlocale(LC_ALL, "en_US.UTF-8");

				initscr();
				// make typed characters immediately available to the program
				cbreak();
				// disable echoing done by ncurses getch() function
				noecho();
				// make cursor invisible
				curs_set(0);
				// wait one tenth of a millisecond for user input
				timeout(0.1);
				// initialize color usage
				start_color();

				init_pair(1, COLOR_RED, 	COLOR_BLACK);
				init_pair(2, COLOR_WHITE, 	COLOR_BLACK);
				init_pair(3, COLOR_GREEN, 	COLOR_BLACK);
				init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
				init_pair(5, COLOR_BLACK, 	COLOR_WHITE);

				main_menu();

				endwin();
				return 0;
			}
