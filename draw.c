#include <ncursesw/curses.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <wchar.h>
#include "game.h"
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

void draw_rectangle(const int y, const int x, const int y_len, const int x_len)
			{
				mvhline(y, 			x, 			0, x_len);
				mvvline(y, 			x, 			0, y_len);
				mvhline(y+y_len, 	x, 			0, x_len);
				mvvline(y, 			x+x_len, 	0, y_len);
			}

void empty_inside(const int y, const int x, const int y_len, const int x_len)
			{
				for ( size_t row = 1; row < y_len; row++ )
					for ( size_t col = 1; col < x_len; col++ )
						mvaddch(y + row, x + col, ' ');
			}

void polish_edges(const int y, const int x, int y_len, int x_len)
			{
				y_len = y_len == 0 ? Y_OFFSET : y_len;
				x_len = x_len == 0 ? X_OFFSET : x_len;

				mvaddch(y, 			x, 			ACS_ULCORNER);
				mvaddch(y, 			x+x_len, 	ACS_URCORNER);
				mvaddch(y+y_len, 	x, 			ACS_LLCORNER);
				mvaddch(y+y_len, 	x+x_len, 	ACS_LRCORNER);
			}

void card_shape(const int y, const int x, const int y_len, const int x_len)
			{
				draw_rectangle 	(y, x, y_len, x_len);
				empty_inside 	(y, x, y_len, x_len);
				polish_edges 	(y, x, y_len, x_len);
			}

void draw_back_of_card(const int y, const int x, const int y_len)
			{
				for ( size_t row = 1; row < y_len; row++ )
				{
					for ( size_t col = 1; col < X_OFFSET; col++ )
					{
						if 		( row % 2 != 0 && col % 2 != 0) mvaddch(y+row, x+col, ACS_CKBOARD | COLOR_PAIR(1));
						else if ( row % 2 == 0 && col % 2 == 0) mvaddch(y+row, x+col, ACS_CKBOARD | COLOR_PAIR(1));
					}
				}
			}

void highlight(const int y, const int x, int y_len, int x_len, const int color_index)
			{
				attron 			(COLOR_PAIR(color_index));
				draw_rectangle 	(y, x, y_len, x_len);

				y_len = y_len == 0 ? Y_OFFSET : y_len;
				x_len = x_len == 0 ? X_OFFSET : x_len;

				polish_edges 	(y, x, y_len, x_len);
				attroff 		(COLOR_PAIR(color_index));
			}

void hidden_highlight(const int y, const int x, const int color)
			{
				attron(COLOR_PAIR(color));
				mvvline(y, x, 			0, 2);
				mvhline(y, x, 			0, X_OFFSET);
				mvvline(y, x+X_OFFSET, 	0, 2);

				mvaddch(y, x, 			ACS_ULCORNER);
				mvaddch(y , x+X_OFFSET, ACS_URCORNER);
				attroff(COLOR_PAIR(color));
			}

void draw_timer(const time_t start, int *score, int *decreases)
			{
				time_t end;
				double time_elapsed, seconds;
				int minutes;

				end 			= clock();
				time_elapsed 	= ((double) (end - start)) / CLOCKS_PER_SEC;
				seconds 		= fmod(time_elapsed, 60.0);
				minutes 		= (int) time_elapsed / 60;

				if ( (int) seconds % 10 == 0 && (int) seconds / 10 != *decreases)
				{
					*score 		= *score - 2 < 0 ? 0 : *score - 2;
					*decreases 	= (int) seconds / 10;
				}

				mvprintw(13, 6, "time:  %02d:%06.3lf", minutes, seconds);
				mvprintw(14, 6, "score: %d   ", *score);
			}

void draw_hint(const int y, const int x, const int y_len, const char hint[], const int pos)
			{
				mvhline(y+pos, x, 0, y_len);

				if ( pos == 0 )
				{
					mvaddch(y+pos, x, 		ACS_LLCORNER);
					mvaddch(y+pos, x+y_len, ACS_LRCORNER);
				}
				else
				{
					mvaddch(y+pos, x, 		ACS_ULCORNER);
					mvaddch(y+pos, x+y_len, ACS_URCORNER);
				}

				mvprintw(y+1-pos, x+y_len/2-strlen(hint)/2, "%s", hint);
			}

void draw_instructions(const int y, const int x)
			{
				draw_rectangle 	(y+1, x, 20, 30);
				polish_edges 	(y+1, x, 20, 30);

				mvprintw(y,    x+9, "INSTRUCTIONS");
				mvprintw(y+2,  x+2, "movement:");
				mvprintw(y+10, x+2, "pick or place card/s: SPACE");
				mvprintw(y+12, x+2, "select multiple: W");
				mvprintw(y+14, x+2, "deselect multiple: S");
				mvprintw(y+16, x+2, "return card/s: e");
				mvprintw(y+18, x+2, "new deal: r");
				mvprintw(y+20, x+2, "quit: q");

				mvaddch(y+4, x+15, '^');
				mvaddch(y+5, x+15, 'w');
				mvaddch(y+7, x+15, 's');
				mvaddch(y+8, x+15, 'v');
				mvaddch(y+6, x+10, '<');
				mvaddch(y+6, x+12, 'a');
				mvaddch(y+6, x+18, 'd');
				mvaddch(y+6, x+20, '>');

				draw_hint(Y_OFFSET + HINT_OFFSET, 	STOCK_X - 1, 								X_OFFSET + 2, 						"STOCK", 		0);
				draw_hint(Y_OFFSET + HINT_OFFSET, 	WASTE_X - 1, 								X_OFFSET + 2, 						"WASTE", 		0);
				draw_hint(Y_OFFSET + HINT_OFFSET, 	COLS - 4*X_OFFSET - 4*CARD_OFFSET - 1, 		4*X_OFFSET + 3*CARD_OFFSET + 2, 	"FOUNDATION", 	0);
				draw_hint(Y_OFFSET + 2*HINT_OFFSET, TABLEAU_MIN_X - 1, 							7*X_OFFSET + 2*6*CARD_OFFSET + 2, 	"TABLEAU", 		1);
			}


void remove_guide(const int y, const int x)
			{
				empty_inside(y-1, x-1, Y_OFFSET+2, X_OFFSET+2);
				highlight(y-2, x, Y_OFFSET, X_OFFSET, 2);
			}

void card_face(const int y, const int x, const int row, const int col, struct cards field[][25])
			{
				card_shape 	(y, x, Y_OFFSET, X_OFFSET);
				mvprintw 	(y+1, x+1, "%s", field[row][col].rank);

				int rank_width = strlen(field[row][col].rank);

				attron 		(COLOR_PAIR(field[row][col].suit_color));
				mvaddwstr 	(y+1, x+2+rank_width, field[row][col].suit);
				attroff 	(COLOR_PAIR(field[row][col].suit_color));
			}

void draw_top_row(struct cards field[][25], const int amount_of_cards[])
			{
				// stock (first from left)
				if ( amount_of_cards[0] == 0 )
				{
					empty_inside 	(ROW_1_Y - 1, 	STOCK_X - 1, 	Y_OFFSET + 2, 	X_OFFSET + 2);
					card_shape 		(ROW_1_Y, 		STOCK_X, 		0, 				0);
				}
				else
				{
					card_shape 			(ROW_1_Y, STOCK_X, Y_OFFSET, X_OFFSET);
					draw_back_of_card 	(ROW_1_Y, STOCK_X, Y_OFFSET);
				}

				// waste (second from left)
				if (amount_of_cards[1] == 0 )
				{
					empty_inside 	(ROW_1_Y - 1, 	WASTE_X - 1, 	Y_OFFSET + 2, 	X_OFFSET + 2);
					card_shape 		(ROW_1_Y, 		WASTE_X, 		0, 				0);
				}
				else
				{
					int waste_col = amount_of_cards[1] == 0 ? 0 : amount_of_cards[1]-1;

					card_face(ROW_1_Y, WASTE_X, 1, waste_col, field);
				}

				// foundation (4 cards on right)
				empty_inside(ROW_1_Y - 1, COLS - 4*X_OFFSET - 4*CARD_OFFSET - 1, Y_OFFSET + 2, 4*X_OFFSET + 4*CARD_OFFSET + 2);

				for ( size_t i = 1; i <= 4; i++ )
				{
					int foundation_x = COLS - i*X_OFFSET - i*CARD_OFFSET;

					if ( amount_of_cards[6-i] == 0 ) 	card_shape 	(ROW_1_Y, foundation_x, 0, 		0);
					else 								card_face 	(ROW_1_Y, foundation_x, 6-i, 	amount_of_cards[6-i]-1, field);
				}
			}

void draw_column(const int col, struct cards field[][25], const int amount_of_cards[])
			{
				int tmp_y, tmp_x, tmp_row;
				tmp_row = 13 - col;
				tmp_y = ROW_2_Y;
				tmp_y += amount_of_cards[tmp_row] == 0 ? 0 : 3;
				tmp_x = COLS - col*X_OFFSET - 2*col*CARD_OFFSET;

				empty_inside(tmp_y-1, tmp_x-1, LINES - tmp_y, X_OFFSET+2);

				if (amount_of_cards[13-col] != 0)
				{
					for ( size_t row = 0; row < amount_of_cards[13-col]; row++ )
					{
						tmp_y = ROW_2_Y + 2*row;
						card_shape(tmp_y, tmp_x, Y_OFFSET, X_OFFSET);

						if ( field[tmp_row][row].revealed == 0 ) 	draw_back_of_card 	(tmp_y, tmp_x, 2);
						else 										card_face 			(tmp_y, tmp_x, tmp_row, row, field);
					}
				}
				else card_shape(tmp_y, tmp_x, 0, 0);
			}

void draw_tableau(struct cards field[][25], const int amount_of_cards[])
			{
				for ( size_t col = 7; col >= 1; col--) draw_column(col, field, amount_of_cards);
			}

void highlight_multiple(const int row, struct cards field[][25], const int amount_of_cards[], const int selected_row, const int selected_col, int *num_of_selected, const int opt)
			{
				int from_row = 13 - (7 - selected_col);
				int from_col = amount_of_cards[from_row] - 1 - *num_of_selected;

				int y = ROW_2_Y + 2*(amount_of_cards[from_row] - 1);
				int x = COLS - (7 - selected_col)*X_OFFSET - 2*(7 - selected_col)*CARD_OFFSET;

				if ( selected_row == 1 && row != 0 )
				{
					if ( opt == 4 && amount_of_cards[from_row] > *num_of_selected && field[from_row][from_col].revealed == 1 )
					{
						hidden_highlight(y - 2*(*num_of_selected), x, opt);
						*num_of_selected += 1;
					}
					else if ( opt == 2 && *num_of_selected > 1 )
					{
						*num_of_selected -= 1;
						hidden_highlight(y - 2*(*num_of_selected), x, opt);
					}
				}
			}

