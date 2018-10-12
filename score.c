#include<stdio.h>
#include<allegro5/allegro_font.h>

#include "score.h"
#include "food.h"
#include "color_names.h"

static int score = 0;

extern ALLEGRO_FONT *font;
extern food f;
extern double time_elapsed;

void update_score(void)
{
	if(f.fq == LITTLE_FOOD)
		score += 5;

	/* Earlier the large prey eaten, higher the score added */
	else if(f.fq == MORE_FOOD)
		score += (25 * (int)(6.0 - time_elapsed));
}

void display_score(void)
{
	char points[6];

	sprintf(points, "%i", score); /* convert score(number) to string */

	al_draw_text(font, ORANGE_COLOR, 50.0, 25.0, ALLEGRO_ALIGN_LEFT, "Score: ");
	al_draw_text(font, YELLOW_COLOR, 150.0, 25.0, ALLEGRO_ALIGN_LEFT, points);
}
