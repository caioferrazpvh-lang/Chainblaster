#ifndef SHIP_H
#define SHIP_H

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

#include "tiro.h"

typedef struct Ship {
	float x, y;
	float vel;	
	ALLEGRO_COLOR cor;
	Tiro tiro;
} Ship;

void initTiro(Ship *s);

#endif