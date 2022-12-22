#include <stdbool.h>

void move_right 		(int *y, int *x, const int row, int *col, const int amount_of_cards[], const int selected_row, const int selected_col, int *select_guide);

void move_left 			(int *y, int *x, const int row, int *col, const int amount_of_cards[], const int selected_row, const int selected_col, int *select_guide);

void move_up 			(int *y, int *x, int *row, int *col, const int amount_of_cards[], const int selected_row, int *select_guide, const int num_of_selected);

void move_down 			(int *y, int *x, int *row, int *col, const int amount_of_cards[], const int selected_row, int *select_guide);

void handle_selected 	(int *y, int *x, int *row, int *col, struct cards field[][25], int *amount_of_cards, int *selected_row, int *selected_col, int *select_guide, int *num_of_selected, int *score);

bool move_valid 		(const int row, const int from_row, const int from_col, const int to_row, const int to_col, struct cards field[][25], const int amount_of_cards[], int *offset);

void finish_move 		(int *y, const int x, const int row, const int col, const int from_row, const int from_col, const int to_row, const int to_col,struct cards field[][25], int *amount_of_cards, int *selected_row, int *selected_col, int *select_guide, int *num_of_selected, int *offset, int *score);

void reveal_stock_card 	(struct cards field[][25], int *amount_of_cards);

void restock_stock 		(struct cards field[][25], int *amount_of_cards, int *score);

void return_card 		(int *y, const int x, const int col, struct cards field[][25], const int amount_of_cards[], int *selected_row, int *selected_col, int *select_guide, int *num_of_selected);
