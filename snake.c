#include<stdlib.h>
#include<allegro5/allegro_primitives.h>

#include "snake.h"
#include "main.h"
#include "motion.h"
#include "color_names.h"

snake_part *s_head = NULL;
snake_part *s_tail = NULL;

extern int m_allocated;
extern int m_deallocated;

extern turn_cors *t_head;
extern turn_cors *prev_tc;

extern bool is_game_over;

void initialize_snake(void)
{
	float y_cor = SCREEN_HEIGHT / 2.0;
	float centre = SCREEN_WIDTH / 2.0;
	float body_part_radius = 5.0;

	snake_part *prev = NULL;
	snake_part *sp = NULL;

	for(int i=0; i<6; ++i)
	{
		++m_allocated;
		sp = malloc(sizeof(snake_part));
		sp->r = body_part_radius;

		sp->sx = (prev) ? (centre + prev->r + sp->r) : centre;
		sp->sy = y_cor;

		sp->color = LIGHT_BLUE_COLOR;
		sp->dir = LEFT;
		sp->next = NULL;

		if(prev)
			prev->next = sp;

		prev = sp;
		centre = sp->sx;

		if(i == 0)
			s_head = sp;

		else if(i == 5)
			s_tail = sp;
	}

	s_head->color = RED_COLOR;
}

void draw_snake(void)
{
	snake_part *i = s_head;
	snake_part *next = NULL;

	float snake_eye_radius = 1.0;

	for(; i != NULL; i = next)
	{
		next = i->next;
		al_draw_filled_circle(i->sx, i->sy, i->r, i->color);
	}

	i = s_head;

	/* draw snake eyes */
	if(i->dir == LEFT || i->dir == RIGHT)
	{
		al_draw_filled_circle(i->sx, i->sy + i->r, snake_eye_radius , WHITE_COLOR);
		al_draw_filled_circle(i->sx, i->sy - i->r, snake_eye_radius, WHITE_COLOR);
	}

	else if(i->dir == UP || i->dir == DOWN)
	{
		al_draw_filled_circle(i->sx + i->r, i->sy, snake_eye_radius , WHITE_COLOR);
		al_draw_filled_circle(i->sx - i->r, i->sy, snake_eye_radius, WHITE_COLOR);
	}

	if(is_game_over)
		al_draw_circle(s_head->sx, s_head->sy, 2 * s_head->r, RED_COLOR, 2.0f);
}

void update_snake_position(void)
{
	float move_speed = 2.0;

	snake_part *i = s_head;
	snake_part *next = NULL;

	for(; i != NULL; i = next)
	{
		next = i->next;

		turn_cors *tc = t_head;
		turn_cors *next_t = NULL;

		/* check every snake part if it goes through turning co-ordinate */
		for(; tc != NULL; tc = next_t)
		{
			next_t = tc->next;

			if(i->sx == tc->sx && i->sy == tc->sy)
			{
				i->dir = tc->dir;

				/* remove the turning co-ordinate once the snake-tail passes through it */
				if(next == NULL)
				{
					tc = t_head;

					/* make the next turning cor the first one */
					if(t_head->next)
						t_head = t_head->next;

					else
					{
						t_head = NULL;
						prev_tc = NULL;
					}

          ++m_deallocated;
					free(tc);
				}
			}
		}

		switch(i->dir)
		{
			case LEFT:
				i->sx -= move_speed;
				break;
			case RIGHT:
				i->sx += move_speed;
				break;
			case UP:
				i->sy -= move_speed;
				break;
			case DOWN:
				i->sy += move_speed;
				break;
		}

	}
}

void check_snake_out_of_arena(void)
{
	snake_part *i = s_head;
	snake_part *next = NULL;

	for(; i != NULL; i = next)
	{
		next = i->next;

		if(((i->sx - i->r) < ARENA_START_X) && i->dir == LEFT)
			i->sx = ARENA_END_X + i->r;

		else if(((i->sx + i->r) > ARENA_END_X) && i->dir == RIGHT)
			i->sx = ARENA_START_X - i->r;

		else if(((i->sy - i->r) < ARENA_START_Y) && i->dir == UP)
			i->sy = ARENA_END_Y + i->r;

		else if(((i->sy + i->r) > ARENA_END_Y) && i->dir == DOWN)
			i->sy = ARENA_START_Y - i->r;
	}

}

int check_snake_collision(void)
{
	snake_part *i = s_head->next;
	snake_part *next = NULL;

	for(; i != NULL; i = next)
	{
		next = i->next;

		/* collision happens only when head direction and body direction is perpendicular to each other, and co-ordinates collides */
		switch(s_head->dir)
		{
			case LEFT:
			case RIGHT: //check for perpendicularity and then for co-ordinates collision
				if(i->dir == UP || i->dir == DOWN)
					if(s_head->sx == i->sx && ((s_head->sy <= i->sy + i->r) && (s_head->sy >= i->sy - i->r)))
						return 1;
				break;
			case UP:
			case DOWN:
				if(i->dir == LEFT || i->dir == RIGHT)
					if(s_head->sy == i->sy && ((s_head->sx <= i->sx + i->r) && (s_head->sx >= i->sx - i->r)))
						return 1;
				break;
		}
	}

	return 0;
}

void append_snake_part(void)
{
	++m_allocated;
	snake_part *sp = malloc(sizeof(snake_part));
	sp->r = s_tail->r;
	sp->color = LIGHT_BLUE_COLOR;
	sp->dir = s_tail->dir;
	sp->next = NULL;

	/* append the new part to the snake tail depending on it's direction */
	switch(s_tail->dir)
	{
		case LEFT:
			sp->sx = s_tail->sx + 2 * s_tail->r;
			sp->sy = s_tail->sy;
			break;
		case RIGHT:
			sp->sx = s_tail->sx - 2 * s_tail->r;
			sp->sy = s_tail->sy;
			break;
		case UP:
			sp->sx = s_tail->sx;
			sp->sy = s_tail->sy + 2 * s_tail->r;
			break;
		case DOWN:
			sp->sx = s_tail->sx;
			sp->sy = s_tail->sy - 2 * s_tail->r;
			break;
	}

	/* make the new part the snake tail */
	s_tail->next = sp;
	s_tail = sp;
}

void destroy_snake(void)
{
	snake_part *i = s_head;
	snake_part *next = NULL;

	for(; i != NULL; i = next)
	{
		++m_deallocated;
		next = i->next;
		free(i);
	}
}
