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
#include "enemies.h"
#include "heros.h"
#include "ship.h"

void initTiro(Ship *s) {

	//printf("\nInit tiro!");

	s->tiro.x = s->x;
	s->tiro.y = s->y;
	s->tiro.raio = 1;
	s->tiro.cor = s->cor;
	s->tiro.timer = TEMPO_TIRO;
	s->tiro.modo = TIRO_INATIVO;

}


