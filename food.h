#pragma once

#include<allegro5/allegro.h>

typedef enum food_qty {LITTLE_FOOD, MORE_FOOD} food_qty;

typedef struct food {
	float sx;
	float sy;
	float r_small;
	float r_big;
	food_qty fq;
	ALLEGRO_COLOR food_color;
} food;

extern void feed_more(void);
extern void check_if_food_is_eaten(void);
extern void handle_large_food_timer(void);
extern void draw_time_elapsed_bar(void);
extern void draw_food(void);
