#include<stdio.h>
#include<stdlib.h>

#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_audio.h>

#include "debug.h"
#include "snake.h"
#include "motion.h"

int m_allocated = 0;
int m_deallocated = 0;

extern ALLEGRO_TIMER *timer;
extern ALLEGRO_TIMER *food_timer;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_FONT *font;
extern ALLEGRO_BITMAP *icon;
extern ALLEGRO_DISPLAY *display;

extern ALLEGRO_SAMPLE *button_click;
extern ALLEGRO_SAMPLE *food_eaten;
extern ALLEGRO_SAMPLE *bg_music;
extern ALLEGRO_SAMPLE *snake_collision;

extern snake_part *s_head;
extern turn_cors *t_head;

void abort_game(const char *err)
{
	cleanup();
	al_show_native_message_box(NULL, "ERROR", "ERROR", err, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	exit(EXIT_FAILURE);
}

/* free up allocated memory */
void cleanup(void)
{
	printf("\nFreeing up memory...");

	if(timer)
		al_destroy_timer(timer);

	if(food_timer)
		al_destroy_timer(food_timer);

	if(event_queue)
		al_destroy_event_queue(event_queue);

	if(font)
		al_destroy_font(font);

	if(icon)
		al_destroy_bitmap(icon);

	if(display)
		al_destroy_display(display);
		
	destroy_sound();

	if(s_head)
		destroy_snake();

	if(t_head)
		destroy_turn_cors();

	fprintf(stdout, "\nmemory allocated = %i blocks\n", m_allocated);
	fprintf(stdout, "memory de-allocated = %i blocks\n", m_deallocated);

	if(m_allocated == m_deallocated)
		fprintf(stdout, "No memory leaks\n");
	else
		fprintf(stdout, "Possible memory leaks\n");

	printf("Done!\n");
	printf("\nGAME OVER!\n");
}

void destroy_sound(void)
{
	if(bg_music)
		al_destroy_sample(bg_music);
		
	if(button_click)
		al_destroy_sample(button_click);
		
	if(food_eaten)
		al_destroy_sample(food_eaten);
		
	if(snake_collision)
		al_destroy_sample(snake_collision);
}
