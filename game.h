struct cards
			{
				char 	*rank;
				wchar_t *suit;
				int 	weight;
				int 	suit_color;
				int 	revealed;
			};

void main_menu 			();

void help 				();

void scoring 			();

void leaderboard 		(const int score);

void solitaire 			(const int y, const int x);

void fill_deck 			(struct cards deck[]);

void fill_field 		(struct cards deck[], struct cards field[][25], const int amount_of_cards[]);

void null_struct_elem 	(const int row, const int col, struct cards field[][25]);

void draw_ascii_title_card(const int y, const int x, const int title_size, const char title[]);
