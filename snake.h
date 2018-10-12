#pragma once

#include<allegro5/allegro.h>
#include "motion.h"

typedef struct snake_part
{
	float sx;
	float sy;
	float r;
	ALLEGRO_COLOR color;
	direction dir;
	struct snake_part *next;
} snake_part;

extern void initialize_snake(void);
extern void draw_snake(void);
extern void update_snake_position(void);
extern void check_snake_out_of_arena(void);
extern int check_snake_collision(void);
extern void append_snake_part(void);
extern void destroy_snake(void);
