#include<stdio.h>
#include<stdlib.h>

#include "motion.h"
#include "snake.h"

turn_cors *t_head = NULL;
turn_cors *prev_tc = NULL;

extern int m_allocated;
extern int m_deallocated;

extern snake_part *s_head;
extern snake_part *s_tail;

void record_turn_cors(int k_code)
{
	turn_cors *i = NULL;

	++m_allocated;

	i = malloc(sizeof(turn_cors));
	if(t_head == NULL)
		t_head = i;

	i->sx = s_head->sx;
	i->sy = s_head->sy;

	if(k_code == ALLEGRO_KEY_LEFT)
	{
		i->dir = LEFT;
		fprintf(stdout, "LEFT\n");
	}

	else if(k_code == ALLEGRO_KEY_RIGHT)
	{
		i->dir = RIGHT;
		fprintf(stdout, "RIGHT\n");
	}

	else if(k_code == ALLEGRO_KEY_UP)
	{
		i->dir = UP;
		fprintf(stdout, "UP\n");
	}

	else if(k_code == ALLEGRO_KEY_DOWN)
	{
		i->dir = DOWN;
		fprintf(stdout, "DOWN\n");
	}

	i->next = NULL;

	if(prev_tc)
		prev_tc->next = i;

	prev_tc = i;
}

void destroy_turn_cors(void)
{
	turn_cors *i = t_head;
	turn_cors *next = NULL;

	for(; i != NULL; i = next)
	{
		++m_deallocated;
		next = i->next;
		free(i);
	}
}
