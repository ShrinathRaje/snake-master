#pragma once

typedef enum direction {LEFT, RIGHT, UP, DOWN} direction;

typedef struct turn_cors
{
	float sx;
	float sy;
	direction dir;
	struct turn_cors *next;
} turn_cors;

extern void record_turn_cors(int);
extern void destroy_turn_cors(void);
