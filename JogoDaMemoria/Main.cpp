#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_memfile.h>
#include <stdio.h>
#include<conio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#define NUM_INTERF 5
#define INTERF_W 100
#define INTERF_H 30
#define NUM_CARDS 16
#define CARD_W 100
#define CARD_H 150
#define NUM_PLAYERS 50

const char* FILENAME = "game_state.sav";


ALLEGRO_BITMAP* back_image = NULL;
int selected_card = -1;
int last_card = -1;
int num_matches = 0;


typedef struct {
	int id;
	int x;
	int y;
	ALLEGRO_BITMAP* hud;
} Interface;

Interface interface[NUM_INTERF];


void init_game_interface(ALLEGRO_BITMAP* interf_images[], int spacing, int screen_w, int screen_h) {
	for (int i = 0; i < NUM_INTERF; i++) {	
			interface[i].id = i % (NUM_INTERF);
			interface[i].hud = interf_images[interface[i].id];
			
			if (i == 0) {
				interface[i].x = (screen_w - INTERF_W * 13.5) / 2 + (i % 4) * (INTERF_W + spacing);
				interface[i].y = (screen_h - INTERF_H * 12) / 2 + (i / 4) * (INTERF_H + spacing);
			}
			if (i == 1) {
				interface[i].x = (screen_w - INTERF_W * 2) / 1.3 + (i % 4) * (INTERF_W + spacing);
				interface[i].y = (screen_h - INTERF_H * 2) / 1.3 + (i / 4) * (INTERF_H + spacing);
			}
			if (i == 2) {
				interface[i].x = (screen_w - INTERF_W * 2) / 1.3 + (INTERF_W + spacing);
				interface[i].y = (screen_h - INTERF_H * 2) / 1.7 + (INTERF_H + spacing);
			}
			if (i == 3) {
				interface[i].x = (screen_w - INTERF_W * 2) / 1.3 + (INTERF_W + spacing);
				interface[i].y = (screen_h - INTERF_H * 2) / 1.94 + (INTERF_H + spacing);
			}
			if (i == 4) {
				interface[i].x = (screen_w - INTERF_W * 2) / 1.3 + (INTERF_W + spacing);
				interface[i].y = (screen_h - INTERF_H * 2) / 2.26 + (INTERF_H + spacing);
			}
	}
}

void draw_interface(Interface interf) {
		al_draw_bitmap(interf.hud, interf.x, interf.y, 0);
}


typedef struct {
	int id;
	int x;
	int y;
	bool is_flipped;
	ALLEGRO_BITMAP* front_image;
	ALLEGRO_BITMAP* back_image;
} Card;

Card game_cards[NUM_CARDS];
Card temp[NUM_CARDS];

typedef struct {
	std::string playerName;
	int score;
	int matches;
	std::vector<Card> cards;
} Player;

Player game_state;

//Função com problema.
void save_game(std::string file_name, Player game_state) {
	// std::string file_name = game_state.playerName + ".sav";
	ALLEGRO_FILE* file = al_fopen(file_name.c_str(), "wb");
	al_fwrite(file, game_state.playerName.c_str(), game_state.playerName.size() + 1);
	al_fwrite(file, &game_state.score, sizeof(int));
	al_fwrite(file, &game_state.matches, sizeof(int));
	int num_cards = game_state.cards.size();
	al_fwrite(file, &num_cards, sizeof(int));
	for (int i = 0; i < num_cards; i++) {
		al_fwrite(file, &game_state.cards[i].id, sizeof(int));
		al_fwrite(file, &game_state.cards[i].x, sizeof(int));
		al_fwrite(file, &game_state.cards[i].y, sizeof(int));
		al_fwrite(file, &game_state.cards[i].is_flipped, sizeof(bool));
	}
	al_fclose(file);
}

//Função com problema, programa crasha ao tentar carregar um arquivo de save.
Player load_game(std::string file_name) {
	Player game_state;
	file_name = game_state.playerName + ".sav";
	ALLEGRO_FILE* file = al_fopen(file_name.c_str(), "rb");
	std::string name;
	al_fread(file, &name, 100);
	//game_state.playerName = name;
	//al_fread(file, &game_state.score, sizeof(int));
	int num_cards = game_state.cards.size();
	al_fread(file, &num_cards, sizeof(int));
	for (int i = 0; i < num_cards; i++) {
		Card card;
		al_fread(file, &card.id, sizeof(int));
		al_fread(file, &card.x, sizeof(int));
		al_fread(file, &card.y, sizeof(int));
		al_fread(file, &card.is_flipped, sizeof(bool));

		game_state.cards.push_back(card);
	}
	al_fclose(file);
	return game_state;
}


std::string get_file_name(ALLEGRO_DISPLAY* display) {
	std::string player_name;
	ALLEGRO_FILECHOOSER* dialog = al_create_native_file_dialog(".", "Load Game", "*.sav", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	if (al_show_native_file_dialog(display, dialog)) {
		player_name = al_get_native_file_dialog_path(dialog, 0);
	}
	al_destroy_native_file_dialog(dialog);
	return player_name;
}

std::string get_player_name() {
	std::string player_name;
	std::cout << "Type your name: ";
	std::cin >> player_name;
	fflush(stdin);
	return player_name;
}

void shuffle(ALLEGRO_BITMAP* card_images[]) {
	for (int i = 0; i < NUM_CARDS; i++) {
		int j = rand() % NUM_CARDS;

		temp[i].id = game_cards[i].id;
		game_cards[i].id = game_cards[j].id;
		game_cards[j].id = temp[i].id;

		temp[i].front_image = game_cards[i].front_image;
		game_cards[i].front_image = game_cards[j].front_image;
		game_cards[j].front_image = temp[i].front_image;
	}
}

void init_game_cards(ALLEGRO_BITMAP* card_images[], int spacing, int screen_w, int screen_h) {
	srand(time(NULL));

	for (int i = 0; i < NUM_CARDS; i++) {
		game_cards[i].id = i % (NUM_CARDS / 2);
		game_cards[i].x = (screen_w - CARD_W * 5) / 2 + (i % 4) * (CARD_W + spacing);
		game_cards[i].y = (screen_h - CARD_H * 5) / 2 + (i / 4) * (CARD_H + spacing);
		game_cards[i].is_flipped = false;
		game_cards[i].front_image = card_images[game_cards[i].id];
	}
	shuffle(card_images);
}


void draw_card(Card card) {
	if (card.is_flipped) {
		al_draw_bitmap(card.front_image, card.x, card.y, 0);
	}
	else {
		al_draw_bitmap(back_image, card.x, card.y, 0);
	}
}

void scoreBoard(int score, std::string nameScoreBoard) {
	ALLEGRO_FONT* font18 = al_load_font("arial.ttf", 24, 0);
	al_draw_textf(font18, al_map_rgb(0, 0, 0), 315, 305, ALLEGRO_ALIGN_CENTRE, "%d", score);
	al_draw_textf(font18, al_map_rgb(0, 0, 0), 315, 275, ALLEGRO_ALIGN_CENTRE, "%s", nameScoreBoard.c_str());
	al_destroy_font(font18);
}

int main(int argc, char** argv) {
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	//Player game_state;
	bool done = false;
	bool redraw = true;

	if (!al_init()) {
		al_show_native_message_box(display, "Error", "Could not initialize Allegro 5", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_install_mouse();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_native_dialog_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	if (!al_install_mouse()) {
		al_show_native_message_box(display, "Error", "Could not initialize the mouse", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if (!al_init_primitives_addon()) {
		al_show_native_message_box(display, "Error", "Could not initialize the primitives addon", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	display = al_create_display(1600, 900);
	if (!display) {
		al_show_native_message_box(display, "Error", "Could not create display", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	int screen_w = al_get_display_width(display);
	int screen_h = al_get_display_height(display);


	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(display, "Error", "Could not create event queue", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return -1;
	}

	timer = al_create_timer(1.0 / 60);
	if (!timer) {
		al_show_native_message_box(display, "Error", "Could not create timer", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		return -1;
	}

	back_image = al_load_bitmap("card_back.png");

	ALLEGRO_BITMAP* interf_images[NUM_INTERF];
	for (int i = 0; i < NUM_INTERF; i++) {
		char filename[256];
		sprintf_s(filename, "interf%d.png", i);
		interf_images[i] = al_load_bitmap(filename);
		if (!interf_images[i]) {
			al_show_native_message_box(display, "Error", "Could not load interface", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			al_destroy_display(display);
			al_destroy_event_queue(event_queue);
			al_destroy_timer(timer);
			for (int j = 0; j < i; j++) {
				al_destroy_bitmap(interf_images[j]);
			}
			return -1;
		}
	}
	init_game_interface(interf_images, 60, screen_w, screen_h);


	ALLEGRO_BITMAP* card_images[NUM_CARDS / 2];
	for (int i = 0; i < NUM_CARDS / 2; i++) {
		char filename[256];
		sprintf_s(filename, "card%d.png", i);
		card_images[i] = al_load_bitmap(filename);
		if (!card_images[i]) {
			al_show_native_message_box(display, "Error", "Could not load card image", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			al_destroy_display(display);
			al_destroy_event_queue(event_queue);
			al_destroy_timer(timer);
			for (int j = 0; j < i; j++) {
				al_destroy_bitmap(card_images[j]);
			}
			return -1;
		}
	}
	init_game_cards(card_images, 50, screen_w, screen_h);

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	int pair = 0; //par de 2 loops
	int score = 0;
	std::string player_name;

	game_state.score = 0;

	std::string nameScoreBoard;
	std::cout << "Type your name: ";
	std::cin >> nameScoreBoard;
	

	while (!done) {
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		game_state.matches = num_matches;

		if (game_state.score <= score) {
			game_state.score = score;
		}


		if (pair == 2) {
			last_card = -1;
			selected_card = -1;
			pair = 0;
		}

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			int x = event.mouse.x;
			int y = event.mouse.y;

			//for -> cartas
			for (int i = 0; i < NUM_CARDS; i++) {
				if (x > game_cards[i].x && x < game_cards[i].x + CARD_W && y > game_cards[i].y && y < game_cards[i].y + CARD_H && game_cards[i].is_flipped == false) {
					pair++;
					if (selected_card == -1) {
						game_cards[i].is_flipped = true;
						redraw = true;
						selected_card = i;
					}
					else if (selected_card != -1 && game_cards[i].is_flipped == false) {
						game_cards[i].is_flipped = true;
						redraw = true;
						last_card = selected_card;
						selected_card = i;
						//Se as cartas forem iguais, entra aqui.
						if (game_cards[i].id == game_cards[last_card].id) {
							score += 125;
							num_matches++;
							game_state.matches = num_matches;
							game_cards[i].is_flipped = true;
							game_cards[last_card].is_flipped = true;
							if (num_matches == (NUM_CARDS / 2)) {
								game_state.score = score;
								al_show_native_message_box(display, "Congratulations", "You have won the game!", "", NULL, ALLEGRO_MESSAGEBOX_WARN);
								num_matches = 0;
							}
						}
						else {
							game_cards[i].is_flipped = true;
							for (int i = 0; i < NUM_CARDS; i++) {
								draw_card(game_cards[i]);
							}
							al_flip_display();
							al_rest(1.0);
							score -= 25;
							game_cards[i].is_flipped = false;
							game_cards[last_card].is_flipped = false;
							last_card = -1;
							selected_card = -1;
						}
					}
				}
			}

			//for -> interface
			for (int i = 0; i < NUM_INTERF; i++) {
				//QUIT
				if (x > interface[1].x && x < interface[1].x + INTERF_W && y > interface[1].y && y < interface[1].y + INTERF_H) {
					done = true;					
				}
				//RESET
				if (x > interface[2].x && x < interface[2].x + INTERF_W && y > interface[2].y && y < interface[2].y + INTERF_H) {
					for (int i = 0; i < NUM_CARDS; i++) {
						num_matches = 0;
						score = 0;
						game_cards[i].is_flipped = false;
						last_card = -1;
						selected_card = -1;
						shuffle(card_images);
					}

				}
				//SAVE *****FAZER*****
				if (x > interface[3].x && x < interface[3].x + INTERF_W && y > interface[3].y && y < interface[3].y + INTERF_H) {
					player_name = get_player_name();
					std::string file_name = player_name + ".sav";
					save_game(file_name, game_state);
				}
				//CHANGE PLAYER *****FAZER*****
				if (x > interface[4].x && x < interface[4].x + INTERF_W && y > interface[4].y && y < interface[4].y + INTERF_H) {
					std::string file_name = get_file_name(display);
					game_state = load_game(file_name);
					std::cout << "Type your name: ";
					std::cin >> nameScoreBoard;
				}
			}
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}



		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(255, 255, 255));
			for (int i = 0; i < NUM_CARDS; i++) {
				draw_card(game_cards[i]);
			}
			for (int i = 0; i < NUM_INTERF; i++) {
				draw_interface(interface[i]);
			}
			scoreBoard(score, nameScoreBoard);
			al_flip_display();
		}
	}
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	for (int i = 0; i < NUM_CARDS / 2; i++) {
		al_destroy_bitmap(card_images[i]);
	}
	return 0;
}


