#include<stdio.h>
#include<time.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>

#include "food.h"
#include "main.h"
#include "snake.h"
#include "score.h"
#include "color_names.h"

double time_elapsed = 0;
food f;

extern snake_part *s_head;
extern bool is_food_eaten;
extern ALLEGRO_TIMER *food_timer;
extern ALLEGRO_SAMPLE *food_eaten;

/* provide food to snake */
void feed_more(void)
{
	static int food_count = 1;

	/* Feed large prey after every 5 small prey */
	f.fq = (food_count <= 5) ? LITTLE_FOOD : MORE_FOOD;

	if(f.fq == LITTLE_FOOD)
	{
		f.r_small = 5.0;
		f.r_big = 7.0;
		f.food_color = LIGHT_BLUE_COLOR;
	}

	else if(f.fq == MORE_FOOD)
	{
		f.r_small = 7.0;
		f.r_big = 10.0;
		f.food_color = RED_COLOR;
	}

	/* generate random poistion for food */
	f.sx = (random() % (int)(ARENA_END_X - ARENA_START_X - 2 * f.r_big)) + ARENA_START_X + f.r_big;
	f.sy = (random() % (int)(ARENA_END_Y - ARENA_START_Y - 2 * f.r_big)) + ARENA_START_Y + f.r_big;

	is_food_eaten = false;

	++food_count;

	/* reset the food_counter after feeding large prey */
	if(food_count > 6)
		food_count = 1;
}

void check_if_food_is_eaten(void)
{
	if(s_head->sx <= f.sx + f.r_small && s_head->sx >= f.sx - f.r_small)
		if(s_head->sy <= f.sy + f.r_small && s_head->sy >= f.sy - f.r_small)
		{
			al_play_sample(food_eaten, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			
			is_food_eaten = true;

			append_snake_part();

			update_score();

			if(f.fq == MORE_FOOD)
			{
				fprintf(stdout, "Woah! That was big! Yummy Yummy...\n");
				al_stop_timer(food_timer);
				time_elapsed = 0.0;
			}

			else
				fprintf(stdout, "That was yummy...ummmm...\n");
		}
}

void handle_large_food_timer(void)
{
	time_elapsed += 0.5;

	if(time_elapsed == 6.0)
	{
		al_stop_timer(food_timer);
		feed_more();
		time_elapsed = 0.0;
		fprintf(stdout, "Oops! Missed that large prey...\n");
	}
}

void draw_time_elapsed_bar(void)
{
	float x;

	x = 450.0 + 10 * (6.0 - time_elapsed); /* time bar of dynamic length */

	al_draw_rounded_rectangle(450.0, 25.0, 510.0, 40.0, 2.0, 2.0,  RED_COLOR, 1.0f);
	al_draw_filled_rounded_rectangle(450.0, 25.0, x, 40.0, 2.0, 2.0, RED_COLOR);
}

void draw_food(void)
{
	static int count = 0;
	int radius;

	radius = (count < 30) ? f.r_small : f.r_big; /* to produce the blinking effect */

	al_draw_filled_circle(f.sx, f.sy, radius, f.food_color);

	++count;

	if(count == 60)
		count = 0;

	if(f.fq == MORE_FOOD)
	{
		al_start_timer(food_timer);
	}
}
