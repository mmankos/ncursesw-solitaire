#include <ncursesw/curses.h>
#include <wchar.h>
#include "game.h"
#include "draw.h"
#include "actions.h"

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

void move_right(int *y, int *x, const int row, int *col, const int amount_of_cards[], const int selected_row, const int selected_col, int *select_guide)
			{
				int row1_min = STOCK_X;
				int row2_min = TABLEAU_MIN_X;

				if ( row == 1 )
				{
					if ( *select_guide == 1 && amount_of_cards[6 + *col] != 0 ) remove_guide(*y, *x);

					*x 	 = *col != 6 ? *x 	+ X_OFFSET + 2*CARD_OFFSET 	: row2_min;
					*col = *col != 6 ? *col + 1 						: 0;

					*y = ROW_2_Y;
					*y += amount_of_cards[6 + *col] == 0 ? 0 : 2*(amount_of_cards[6 + *col] - 1);
					*y += selected_col == -1 ? 0 : amount_of_cards[6 + *col] == 0 ?  0 : 2;

					if ( selected_row == 1 )
						*select_guide = selected_col == *col ? 0 : selected_col == -1 ? 0 : 1;
				}
				else if ( row == 0 )
				{
					*x 	 = *col != 5 ? *x + X_OFFSET + CARD_OFFSET : *select_guide == 1 ? COLS - 4*X_OFFSET - 4*CARD_OFFSET: row1_min;
					*x 	 = *col == 1 ? COLS - 4*X_OFFSET - 4*CARD_OFFSET : *x;
					*col = *col != 5 ? *col+1 : *select_guide == 1 ? 2 : 0;
				}
			}

void move_left(int *y, int *x, const int row, int *col, const int amount_of_cards[], const int selected_row, const int selected_col, int *select_guide)
			{
				int row1_max = COLS - X_OFFSET - CARD_OFFSET;
				int row2_max = COLS - X_OFFSET - 2*CARD_OFFSET;

				if ( row == 1 )
				{
					if ( *select_guide == 1 && amount_of_cards[6 + *col] != 0 ) remove_guide(*y, *x);

					*x 	 = *col != 0 ? *x 	- X_OFFSET - 2*CARD_OFFSET 	: row2_max;
					*col = *col != 0 ? *col - 1  						: 6;

					*y = ROW_2_Y;
					*y += amount_of_cards[6 + *col] == 0 ? 0 : 2*(amount_of_cards[6 + *col] - 1);
					*y += selected_col == -1 ? 0 : amount_of_cards[6 + *col] == 0 ? 0 : 2;

					if ( selected_row == 1 )
						*select_guide = selected_col == *col ? 0 : selected_col == -1 ? 0 : 1;
				}
				else if ( row == 0 )
				{
					*x   = *col == 0 ? row1_max : *x - X_OFFSET - CARD_OFFSET;
					*x 	 = *col != 2 ? *x : *select_guide == 1 ? COLS - X_OFFSET - CARD_OFFSET : WASTE_X;
					*col = *col == 0 ? 5 : *col != 2 ? *col - 1 : *select_guide == 1 ? 5 : *col - 1;
				}
			}

void move_up(int *y, int *x, int *row, int *col, const int amount_of_cards[], const int selected_row, int *select_guide, const int num_of_selected)
			{
				if ( *row == 1 && num_of_selected == 1 )
				{
					if ( *select_guide == 1 && amount_of_cards[6 + *col] != 0 )
						remove_guide(*y, *x);

					*select_guide = selected_row != -1 ? 1 : 0;

					*y = 1;
					*x = *select_guide == 1 ? COLS - 4*X_OFFSET - 4*CARD_OFFSET : CARD_OFFSET;

					*col = *select_guide == 1 ? 2 : 0;
					*row = 0;

				}
			}

void move_down(int *y, int *x, int *row, int *col, const int amount_of_cards[], const int selected_row, int *select_guide)
			{
				if ( *row == 0 )
				{
					*y = ROW_2_Y;
					*y += amount_of_cards[6] == 0 ? 0 : 2*(amount_of_cards[6] - 1);
					*y += selected_row == -1 ? 0 : amount_of_cards[6] == 0 ? 0 : 2;
					*x = TABLEAU_MIN_X;

					*col = 0;
					*row = 1;

					*select_guide = selected_row != -1 ? 1 : 0;
				}
			}

bool move_valid(const int row, const int from_row, const int from_col, const int to_row, const int to_col, struct cards field[][25], const int amount_of_cards[], int *offset)
			{
				if ( amount_of_cards[to_row] == 0 )
				{
					if ( row == 0 && field[from_row][from_col].weight == 1 )
						return true;
					else if ( row == 1 && field[from_row][from_col].weight == 13 )
						return true;
				}
				else if ( amount_of_cards[to_row] > 0 )
				{
					int from_card_weight 	 = field[from_row][from_col].weight;
					int to_card_weight 		 = field[to_row][to_col].weight;
					int suits_matching 		 = wcscmp(field[from_row][from_col].suit,field[to_row][to_col].suit);
					int suit_colors_matching = field[from_row][from_col].suit_color == field[to_row][to_col].suit_color ? 0 : 1;

					if ( row == 0 &&  suits_matching == 0 && from_card_weight - 1 == to_card_weight)
					{
						*offset = 1;
						return true;
					}
					else if ( row == 1 && suit_colors_matching != 0 && from_card_weight == to_card_weight-1 )
					{
						*offset = 1;
						return true;
					}
				}
				return false;
			}

void return_card(int *y, const int x, const int col, struct cards field[][25], const int amount_of_cards[], int *selected_row, int *selected_col, int *select_guide, int *num_of_selected)
			{
				if ( *selected_row != -1 )
				{
					int s_y = *selected_row == 1 ? ROW_2_Y + 2*(amount_of_cards[13 - (7 - *selected_col)] - 1) : 1;
					int s_x;

					if ( *selected_row == 1 )
						s_x = COLS - (7 - *selected_col)*X_OFFSET - 2*(7 - *selected_col)*CARD_OFFSET;
					else if ( *selected_row == 0 )
						s_x = *selected_col == 1 ? WASTE_X : COLS - (6 - *selected_col)*X_OFFSET - (6 - *selected_col)*CARD_OFFSET;

					highlight(s_y, s_x, Y_OFFSET, X_OFFSET, 2);

					if ( *num_of_selected > 1 )
						draw_column(7 - *selected_col, field, amount_of_cards);

					if ( *y != 1  && !(*y == s_y && x == s_x) && amount_of_cards[6 + col] != 0 )
					{
						remove_guide(*y, x);
						*y -= 2;
					}

					*selected_row 		= -1;
					*selected_col 		= -1;
					*select_guide 		= 0;
					*num_of_selected 	= 1;
				}
			}

void finish_move(int *y, const int x, const int row, const int col, const int from_row, const int from_col, const int to_row, const int to_col,struct cards field[][25], int *amount_of_cards, int *selected_row, int *selected_col, int *select_guide, int *num_of_selected, int *offset, int *score)
			{
				int cards_to_move = *num_of_selected;
				for ( size_t i = 0; i < *num_of_selected; i++ )
				{
					field[to_row][to_col+*offset+i] = field[from_row][from_col+i];
					null_struct_elem(from_row, from_col+i, field);
				}
				return_card(y, x, col, field, amount_of_cards, selected_row, selected_col, select_guide, num_of_selected);


				if 		( amount_of_cards[to_row] != 0 && row == 1 ) *y += 2*(cards_to_move);
				else if ( amount_of_cards[to_row] == 0 && row == 1 ) *y += 2*(cards_to_move - 1);

				amount_of_cards[from_row] -= cards_to_move;
				amount_of_cards[to_row]   += cards_to_move;

				if ( to_row < 6 && (from_row == 1 || from_row > 5) )
					*score += 10;
				else if ( from_row == 1 )
					*score += 5;
				else if ( amount_of_cards[from_row] != 0 && (from_row == 1 || from_row > 5) )
				{
					if ( field[from_row][from_col-1].revealed == 0 )
						*score += 5;
				}
				else if ( from_row < 6  && to_row > 5)
					*score = *score - 15 < 0 ? 0 : *score - 15;

				field[from_row][from_col-1].revealed = 1;

				if ( to_row < 6 ) 	draw_top_row(field, amount_of_cards);
				else 				draw_column(13-to_row, field, amount_of_cards);

				if ( from_row > 5 ) draw_column(13-from_row, field, amount_of_cards);
				else 				draw_top_row(field, amount_of_cards);
			}

void reveal_stock_card(struct cards field[][25], int *amount_of_cards)
			{
				int stock_col = amount_of_cards[0] - 1;
				int waste_col = amount_of_cards[1];

				amount_of_cards[1]++;
				amount_of_cards[0]--;

				field[1][waste_col] = field[0][stock_col];
				null_struct_elem(0, stock_col, field);

				draw_top_row(field, amount_of_cards);
			}

void restock_stock(struct cards field[][25], int *amount_of_cards, int *score)
			{
                if ( amount_of_cards[1] != 0 )
                {
                    // move all cards from waste back into stock
                    amount_of_cards[0] = amount_of_cards[1];
                    amount_of_cards[1] = 0;

                    for ( size_t i = amount_of_cards[0]; i > 0; i-- )
                    {
                        field[0][amount_of_cards[0] - i] = field[1][i - 1];
                        null_struct_elem(1, i - 1, field);
                    }

                    draw_top_row(field, amount_of_cards);

                    *score = *score - 100 < 0 ? 0 : *score - 100;
                }
			}


void handle_selected(int *y, int *x, int *row, int *col, struct cards field[][25], int *amount_of_cards, int *selected_row, int *selected_col, int *select_guide, int *num_of_selected, int *score)
			{
				if ( *col == 0 && *row == 0 )
					amount_of_cards[0] != 0 ? reveal_stock_card(field, amount_of_cards) : restock_stock(field, amount_of_cards, score);
				else if ( !(*selected_row == *row && *selected_col == *col) && *select_guide != 1 && amount_of_cards[*col + *row*6] != 0 )
				{
					highlight(*y, *x, Y_OFFSET, X_OFFSET, 4);
					*selected_row = *row;
					*selected_col = *col;
					if ( *row == 0 ) move_down(y, x, row, col, amount_of_cards, *selected_row, select_guide);
				}
				else if ( !(*selected_row == *row && *selected_col == *col) && *select_guide == 1 )
				{
					int from_row = *selected_row == 1 ? 6 + *selected_col : *selected_col;
					int from_col = amount_of_cards[from_row] == 0 ? 0 : amount_of_cards[from_row] - *num_of_selected;

					int to_row = *row == 0 ? *col : 6 + *col;
					int to_col = amount_of_cards[to_row] == 0 ? 0 : amount_of_cards[to_row] - 1;

					int offset = 0;

					if ( move_valid(*row, from_row, from_col, to_row, to_col, field, amount_of_cards, &offset) )
						finish_move(y, *x, *row, *col, from_row, from_col, to_row, to_col, field, amount_of_cards, selected_row, selected_col, select_guide, num_of_selected, &offset, score);
				}
			}

