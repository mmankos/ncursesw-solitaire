#include <time.h>

void draw_rectangle 	(const int y, const int x, const int y_len, const int x_len);

void empty_inside 		(const int y, const int x, const int y_len, const int x_len);

void polish_edges 		(const int y, const int x, int y_len, int x_len);

void card_shape 		(const int y, const int x, const int y_len, const int x_len);

void draw_back_of_card 	(const int y, const int x, const int y_len);

void highlight 			(const int y, const int x, int y_len, int x_len, const int color_index);

void hidden_highlight 	(const int y, const int x, const int color);

void remove_guide 		(const int y, const int x);

void draw_hint 			(const int y, const int x, const int y_len, const char hint[], const int pos);

void draw_instructions 	(const int y, const int x);

void draw_timer 		(const time_t start, int *score, int *decreases);

void card_face 			(const int y, const int x, const int row, const int col, struct cards field[][25]);

void draw_top_row 		(struct cards field[][25], const int amount_of_cards[]);

void draw_tableau 		(struct cards field[][25], const int amount_of_cards[]);

void draw_column 		(const int col, struct cards field[][25], const int amount_of_cards[]);

void highlight_multiple (const int row, struct cards field[][25], const int amount_of_cards[], const int selected_row, const int selected_col, int *num_of_selected, const int opt);
