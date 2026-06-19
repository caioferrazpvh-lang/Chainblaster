#ifndef TIRO_H
#define TIRO_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "config.h"

typedef struct Tiro {
	int x, y;
	float raio;
	int modo;
	float timer;
	ALLEGRO_COLOR cor;
} Tiro;

#endif