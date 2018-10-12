#include<stdlib.h>
#include<time.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

#include "main.h"
#include "snake.h"
#include "motion.h"
#include "food.h"
#include "score.h"
#include "debug.h"
#include "color_names.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_DISPLAY_MODE disp_data;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *food_timer = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *icon = NULL;

/* for sound */
ALLEGRO_SAMPLE *button_click = NULL;
ALLEGRO_SAMPLE *food_eaten = NULL;
ALLEGRO_SAMPLE *bg_music = NULL;
ALLEGRO_SAMPLE *snake_collision = NULL;
ALLEGRO_SAMPLE_ID bg_music_id;

bool is_game_over = false;
bool is_food_eaten = true;

extern snake_part *s_head;

static void initialize_game(void);
static void initialize_display(void);
static void initialize_font(void);
static void initialize_sound(void);
static void draw_arena(void);
static void game_loop(void);
static void process_input_from_keyboard(ALLEGRO_EVENT *);
static void display_game_over(void);

int main(int argc, char **argv)
{
	/* install allegro_libraries and other addons */
	initialize_game();

	draw_arena();

	/* create the snake */
	initialize_snake();

	draw_snake();

	al_flip_display();

	/* wait for 2 seconds */
	al_rest(2.0);

	/* let the game begin */
	game_loop();

	if(is_game_over)
		al_rest(3.0);

	/* free up dynamically allocated memory */
	cleanup();

	return 	EXIT_SUCCESS;
}

void initialize_game(void)
{
	if(!al_init())
		abort_game("Failed to initialize allegro");

	if(!al_install_keyboard())
		abort_game("Failed to install keyboard");

	if(!al_init_primitives_addon())
		abort_game("Failed to install primitives addon");

	if(!al_init_image_addon())
		abort_game("Failed to install image addon");

	timer = al_create_timer(1.0 / FPS);
	if(!timer)
		abort_game("Failed to create timer");

	/* timer for monitoring large/more food */
	food_timer = al_create_timer(0.5);
		if(!food_timer)
			abort_game("Failed to create food timer");

	event_queue = al_create_event_queue();
	if(!event_queue)
		abort_game("Failed to create event queue");

	initialize_display();

	initialize_font();
	
	initialize_sound();

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_timer_event_source(food_timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void initialize_display()
{
	icon = al_load_bitmap("images/Snake-icon.png");
	if(!icon)
		abort_game("Failed to load display icon");

	al_get_display_mode(0, &disp_data);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_new_window_title("Snake-Master");
	al_set_new_window_position((disp_data.width - SCREEN_WIDTH) / 2.0, (disp_data.height - SCREEN_HEIGHT) / 2.0);

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if(!display)
		abort_game("Failed to create display");

	al_set_display_icon(display, icon);
}

void initialize_font(void)
{
	if(!al_init_font_addon())
		abort_game("Failed to initialize font addon");

	if(!al_init_ttf_addon())
		abort_game("Failed to initialize ttf addon");

	font = al_load_ttf_font("fonts/pirulen.ttf", 18, 0);
	if(!font)
		abort_game("Failed to load pirulen.ttf font");
}

void initialize_sound(void)
{
	if(!al_install_audio())
		abort_game("Failed to initialize audion");

	if(!al_init_acodec_addon())
		abort_game("Failed to initialize audio codecs");

	if(!al_reserve_samples(4))
		abort_game("Failed to reserve samples\n");

	bg_music = al_load_sample("sounds/bg_music.wav");
	button_click = al_load_sample("sounds/button_click.wav");
	food_eaten = al_load_sample("sounds/food_eaten.wav");
	snake_collision = al_load_sample("sounds/snake_collision.wav");
}

void draw_arena(void)
{
	al_clear_to_color(BLACK_COLOR);
	al_draw_rectangle(ARENA_START_X, ARENA_START_Y, ARENA_END_X, ARENA_END_Y, GREEN_COLOR, ARENA_BORDER_THICKNESS);
}

void game_loop(void)
{
	bool end_game = false;
	bool draw = false;

	srandom(time(0));

	ALLEGRO_EVENT event;
	
	al_play_sample(bg_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &bg_music_id);

	al_start_timer(timer);

	while( ! end_game )
	{
		al_wait_for_event(event_queue, &event);

		if(event.type == ALLEGRO_EVENT_TIMER)
		{
			if(event.timer.source == timer)
			{
				if(is_food_eaten)
					feed_more();

				update_snake_position();

				check_snake_out_of_arena();

				if(check_snake_collision())
				{
					end_game = true;
					is_game_over = true;
					al_play_sample(snake_collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					al_stop_sample(&bg_music_id);
				}

				check_if_food_is_eaten();

				draw = true;
			}

			else if(event.timer.source == food_timer)
			{
				handle_large_food_timer();
			}

		}

		else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			process_input_from_keyboard(&event);
		}

		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			end_game = true;

		if(draw && al_is_event_queue_empty(event_queue))
		{
			draw_arena();

			draw_snake();

			if( ! is_food_eaten)
				draw_food();

			display_score();

			if(al_get_timer_started(food_timer))
				draw_time_elapsed_bar();

			if(is_game_over)
				display_game_over();

			al_flip_display();

			draw = false;
		}
	}
}

void process_input_from_keyboard(ALLEGRO_EVENT *ev)
{
	int k_code = ev->keyboard.keycode;

	/* disable the direction parallel to the current direction of the snake */
	switch(s_head->dir)
	{
		case LEFT:
		case RIGHT:
			if(k_code == ALLEGRO_KEY_UP || k_code == ALLEGRO_KEY_DOWN)
				record_turn_cors(k_code); //record turning co-ordinate
			break;
		case UP:
		case DOWN:
			if(k_code == ALLEGRO_KEY_LEFT || k_code == ALLEGRO_KEY_RIGHT)
				record_turn_cors(k_code);
			break;
	}
}

void display_game_over(void)
{
	font = al_load_ttf_font("fonts/pirulen.ttf", 30, 0);
	al_draw_text(font, RED_COLOR, SCREEN_WIDTH / 2.0, ARENA_END_Y - 150.0, ALLEGRO_ALIGN_CENTER, "GAME OVER");
}
