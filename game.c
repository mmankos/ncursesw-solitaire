#include <ncursesw/curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <wchar.h>
#include "game.h"
#include "actions.h"
#include "draw.h"

#define X_OFFSET 		7
#define Y_OFFSET 		5
#define CARD_OFFSET 	5
#define HINT_OFFSET 	2
#define STD_SIZE 		10

#define ROW_1_Y 		1
#define ROW_2_Y 		Y_OFFSET + 3*HINT_OFFSET
#define WASTE_X  		X_OFFSET + 2*CARD_OFFSET
#define STOCK_X 		CARD_OFFSET
#define TABLEAU_MIN_X 	COLS - 7*X_OFFSET - 2*7*CARD_OFFSET

struct highscore
			{
				char 	name[21];
				int 	score;
			};

void draw_ascii_title_card(const int y, const int x, const int title_size, const char title[])
            {
                srand(time(NULL));
                char ascii_cards[6][4][20] =
                {
                    {".------.", ".------.", ".------.", ".------."},
                    {"|#.--. |", "|#.--. |", "|#.--. |", "|#.--. |"},
                    {"| :/\\: |", "| (\\/) |", "| :/\\: |", "| :(): |"},
                    {"| :\\/: |", "| :\\/: |", "| (__) |", "| ()() |"},
                    {"| '--'#|", "| '--'#|", "| '--'#|", "| '--'#|"},
                    {"`------'", "`------'", "`------'", "`------'"}
                };

                for ( size_t i = title_size; i > 0; i-- )
                {
                    int random_ascii_card = rand() % 4;
                    for ( size_t j = 6; j > 0; j-- )
                    {
                        mvprintw(y - j, x - ( 8*title_size/2 - 8*(i-1) ), "%s", ascii_cards[6-j][random_ascii_card]);

                        if      ( j == 5 )  mvprintw(y - j, x - ( 8*title_size/2 - 8*(i-1) ) + 1, "%c", title[i-1]);
                        else if ( j == 2 )  mvprintw(y - j, x - ( 8*title_size/2 - 8*(i-1) ) + 6, "%c", title[i-1]);
                    }
                }
            }

void main_menu()
			{
				empty_inside(0, 0, LINES, COLS);
				char input = '#';
				char options[4][15] = {"PLAY", "HELP", "LEADERBOARD", "QUIT"};
				int selected = 0;

                draw_ascii_title_card((LINES - (LINES / 2) + 5) / 2, COLS / 2, strlen("SOLITAIRE"), "SOLITAIRE");
				while ( input != ' ' )
				{
					for ( size_t i = 0; i < 4; i++ )
					{
						if ( selected == i ) attron(COLOR_PAIR(5));
						mvprintw((LINES / 2) - 5 + 3*i, (COLS / 2) - (strlen(options[i]) / 2), "%s", options[i]);
						if ( selected == i ) attroff(COLOR_PAIR(5));
					}
					mvprintw((LINES / 2) + 6, (COLS / 2) - (strlen("^: w, v: s, confirm: SPACE") / 2), "^: w, v: s, confirm: SPACE");

					refresh();
					input = getchar();

					switch (input)
					{
						case 'w':
							selected = selected == 0 ? 3 : selected - 1;
							break;
						case 's':
							selected = selected == 3 ? 0 : selected + 1;
							break;
					}
				}

				switch (selected)
				{
					case 0:
						solitaire(ROW_2_Y, TABLEAU_MIN_X);
						break;
					case 1:
						help();
						break;
					case 2:
						leaderboard(-1);
						break;
				}
			}

void help()
			{
				empty_inside(0, 0, LINES, COLS);

                draw_ascii_title_card((LINES - (LINES / 2) + 6) / 2, COLS / 2, strlen("HELP"), "HELP");

				FILE* ptr = fopen("help.txt", "r");
				if (ptr == NULL)
				{
					mvprintw((LINES / 2) - 1, (COLS / 2) - (strlen("HELP FILE MISSING") / 2),"HELP FILE MISSING");
					return;
				}

				char help_line[110], input = '#';
				int count = 0;

				while ( fscanf(ptr, "%[^\n]%*c", help_line) == 1 )
				{
					mvprintw((LINES / 2) - 6 + count*2, (COLS / 2) - 55, "%s\n\n", help_line);
					count++;
				}
				mvprintw((LINES / 2) + 8, (COLS / 2) - (strlen("scoring: s, return to menu: SPACE") / 2), "scoring: s, return to menu: SPACE");

				refresh();
				fclose(ptr);

				while ( input != ' ' )
				{
					input = getchar();
					if ( input == 's' )
					{
						scoring();
						break;
					}
				}

				if ( input == ' ' ) main_menu();
			}

void scoring()
			{
				empty_inside(0, 0, LINES, COLS);

                draw_ascii_title_card((LINES - (LINES / 2) + 6) / 2, COLS / 2, strlen("SCORING"), "SCORING");

				FILE* ptr = fopen("scoring.txt", "r");
				if (ptr == NULL)
				{
					mvprintw((LINES / 2) - 1, (COLS / 2) - (strlen("SCORING FILE MISSING") / 2),"SCORING FILE MISSING");
					return;
				}

				char scoring_line[66], input = '#';
				int count = 0;

				while ( fscanf(ptr, "%[^\n]%*c", scoring_line) == 1 )
				{
					mvprintw((LINES / 2) - 6 + count*2, (COLS / 2) - 32, "%s\n\n", scoring_line);
					count++;
				}
				mvprintw((LINES / 2) + 8, (COLS / 2) - (strlen("help: h, return to menu: SPACE") / 2), "help: h, return to menu: SPACE");

				refresh();
				fclose(ptr);

				while ( input != ' ' )
				{
					input = getchar();
					if ( input == 'h' )
					{
						help();
						break;
					}
				}
				if ( input == ' ' ) main_menu();
			}

void leaderboard(const int score)
			{
				empty_inside(0, 0, LINES, COLS);

                draw_ascii_title_card((LINES - (LINES / 2) + 5) / 2, COLS / 2, strlen("LEADERBOARD"), "LEADERBOARD");

				FILE* ptr = fopen("leaderboard.txt", "r");
				if (ptr == NULL)
				{
					mvprintw((LINES / 2) - 1, (COLS / 2) - (strlen("LEADERBOARD FILE MISSING") / 2),"LEADERBOARD FILE MISSING");
					return;
				}

				struct highscore top10[10];
				char names[21], scores[10], input = '#';
				int count = 0, pos = 0;

				while ( fscanf(ptr, "%s %s", names, scores) == 2 )
				{
					strncpy(top10[count].name, names, 20);
					top10[count].score 	= strtol(scores, NULL, 10);
					count++;
				}

				for ( size_t i = 0; i < 10; i++ )
				{
					if ( top10[i].score < score && pos == 0 )
					{
						pos = i + 1;

						for ( size_t j = 9; j > i; j-- )
						{
							strncpy(top10[j].name, top10[j-1].name, 20);
							top10[j].score 	= top10[j-1].score;
						}

						attron(COLOR_PAIR(5));
						mvprintw((LINES / 2) - 5 + i, (COLS / 2) - 16, "%2ld. %20s %5d\n", i+1, "............", score);
						attroff(COLOR_PAIR(5));
					}
					else
					{
						mvprintw((LINES / 2) - 5 + i, (COLS / 2) - 16, "%2ld. %20s %5d\n", i+1, top10[i].name, top10[i].score);
					}
				}

				mvprintw((LINES / 2) + 6, (COLS / 2) - (strlen("return to menu: SPACE") / 2), "return to menu: SPACE");

				fclose(ptr);

				if ( score >= 0 )
				{
					int y = (LINES / 2) + ((LINES - (LINES / 2) - 6) / 2);
					int x;
					if ( score > strtol(scores, NULL, 10) )
					{
						x = (COLS / 2) - (strlen("CONGRATULATIONS YOUR SCORE OF ..... IS NOW TOP .. IN THE LEADERBOARD!") / 2);
						mvprintw(y, x, "CONGRATULATIONS YOUR SCORE OF %5d IS NOW TOP %2d IN THE LEADERBOARD!", score, pos);

						x = (COLS / 2) - (strlen("PLEASE ENTER YOUR NAME (MAX 20 CHARACTERS, NO SPACES) AND HIT ENTER!") / 2);
						y += 2;
						mvprintw(y, x, "PLEASE ENTER YOUR NAME (MAX 20 CHARACTERS, NO SPACES) AND HIT ENTER!");

						nodelay(stdscr, FALSE);
						echo();
						refresh();
						top10[pos-1].score = score;
						mvscanw(y + 2, (COLS / 2) - 10, "%s", top10[pos-1].name);
						nodelay(stdscr, TRUE);
						noecho();

                        FILE* ptr_w = fopen("leaderboard.txt", "w");
                        if (ptr_w == NULL)
                        {
                            mvprintw((LINES / 2) - 1, (COLS / 2) - (strlen("LEADERBOARD FILE MISSING") / 2),"LEADERBOARD FILE MISSING");
                            return;
                        }

                        for ( size_t i = 0; i < 10; i++ )
                        {
                            fputs(top10[i].name, ptr_w);
                            fputc(' ', ptr_w);

                            char str_score[10];
                            sprintf(str_score, "%d", top10[i].score);
                            fputs(str_score, ptr_w);
                            fputs("\n", ptr_w);
                        }

                        fclose(ptr_w);
                        main_menu();
                        return;
					}
					else
					{
						x = (COLS / 2) - (strlen("YOUR SCORE OF ..... WAS NOT ENOUGH TO GET TO THE LEADERBOARD. BETTER LUCK NEXT TIME!") / 2);
						mvprintw(y, x, "YOUR SCORE OF %5d WAS NOT ENOUGH TO GET TO THE LEADERBOARD. BETTER LUCK NEXT TIME!", score);
					}
                    refresh();
				}

				refresh();

				while ( input != ' ' ) input = getchar();
				main_menu();
			}

void solitaire(int y, int x)
			{
				int amount_of_cards[] = {24,0,0,0,0,0,1,2,3,4,5,6,7};

				struct cards deck[52];
				fill_deck(deck);

				struct cards field[13][25];
				fill_field(deck, field, amount_of_cards);
				char input = '#', throwaway = '#';

				int col = 0 ,row = 1, y_len = 0, x_len = 0, score = 0, decreases = 0, win = 0;
				int selected_row = -1, selected_col = -1, select_guide = 0, num_of_selected = 1;

				empty_inside(0, 0, LINES, COLS);
				draw_top_row(field, amount_of_cards);
				draw_tableau(field, amount_of_cards);

				draw_instructions(17,6);
				mvprintw(13, 11, "PRESS SPACE TO START...");
				refresh();

				while (throwaway != ' ' )
				{
					throwaway = getchar();
					if ( throwaway == 'q' )
					{
						input = 'q';
						break;
					}
				}

				mvprintw(13, 11, "                       ");

				clock_t start;
				start = clock();

				while ( input != 'q' )
				{
					y_len = amount_of_cards[col+row*6] == 0 ? 0 : Y_OFFSET;
					x_len = amount_of_cards[col+row*6] == 0 ? 0 : X_OFFSET;

					// green highlight for card hovering over
					if( selected_row != row || selected_col != col ) highlight(y, x, y_len, x_len, 3);

					draw_timer(start, &score, &decreases);

					refresh();
					input = getch();

					// remove green highlight from card previously hovered over after input result
					if( selected_row != row || selected_col != col ) highlight(y, x, y_len, x_len, 2);

					if ( amount_of_cards[2] == 13 && amount_of_cards[3] == 13 && amount_of_cards[4] == 13 && amount_of_cards[5] == 13 )
					{
						win = 1;
						break;
					}

					// handle user input
					switch (input)
					{
						case 'd':
							move_right(&y, &x, row, &col, amount_of_cards, selected_row, selected_col, &select_guide);
							break;
						case 'a':
							move_left(&y, &x, row, &col, amount_of_cards, selected_row, selected_col, &select_guide);
							break;
						case 'w':
							move_up(&y, &x, &row, &col, amount_of_cards, selected_row, &select_guide, num_of_selected);
							break;
						case 's':
							move_down(&y, &x, &row, &col, amount_of_cards, selected_row, &select_guide);
							break;
						case ' ':
							handle_selected(&y, &x, &row, &col, field, amount_of_cards, &selected_row, &selected_col, &select_guide, &num_of_selected, &score);
							break;
						case 'e':
							return_card(&y, x, col, field, amount_of_cards, &selected_row, &selected_col, &select_guide, &num_of_selected);
							break;
						case 'W':
							highlight_multiple(row, field, amount_of_cards, selected_row, selected_col, &num_of_selected, 4);
							break;
						case 'S':
							highlight_multiple(row, field, amount_of_cards, selected_row, selected_col, &num_of_selected, 2);
							break;
						case 'r':
							input = 'q';
							win = -1;
							solitaire(ROW_2_Y, TABLEAU_MIN_X);
							break;
					}
				}

				if 		( win == 1 ) leaderboard(score);
				else if ( win == 0 ) main_menu();
			}

void fill_deck(struct cards deck[])
			{
				char *ranks[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
				wchar_t *suits[] = {L"\x2665",L"\x2666",L"\x2660",L"\x2663"};

				for ( size_t i = 0; i < 52; i++ )
				{
					deck[i].rank 		= ranks[i%13];
					deck[i].suit 		= suits[i/13];
					deck[i].weight 		= (i % 13) + 1;
					deck[i].suit_color 	= i/13 > 1 ? 2 : 1;
					deck[i].revealed 	= 0;
				}
			}

void fill_field(struct cards deck[], struct cards field[][25], const int amount_of_cards[])
			{
				srand(time(NULL));

				int index, count = 52;
				int available[count];

				for ( size_t i = 0; i < count; i++ ) available[i] = 0;

				for ( size_t row = 0; row < 13; row++ )
				{
					for ( size_t col = 0; col < 25; col++ )
					{
						if ( col < amount_of_cards[row] )
						{
							index = rand() % count;
							while ( available[index] != 0 ) index = rand() % count;

							field[row][col] = deck[index];
							if ( col == amount_of_cards[row]-1 || row == 0 ) field[row][col].revealed = 1;
							available[index]++;
						}
						else null_struct_elem(row, col, field);
					}
				}
			}

void null_struct_elem(const int row, const int col, struct cards field[][25])
			{
				field[row][col].rank 		= NULL;
				field[row][col].suit 		= NULL;
				field[row][col].weight 		= 0;
				field[row][col].suit_color 	= 0;
				field[row][col].revealed 	= 0;
			}

