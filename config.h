#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RAIO_ASTEROIDE 128
#define VELOCIDADE_HEROI 12
#define NUM_ENEMIES 20
#define TIRO_INATIVO 0
#define TIRO_ATIVO 1
#define RAIO_TIRO 150
#define TEMPO_TIRO 2.0
#define BORDA_TIRO 4
#define SCORE_PENALTY 0.3
#define RAIO_HERO 60
#define FPS 60.0
#define RAIO_INIMIGO 50
#define CHAIN_DELAY 0.05
#define CHAIN_TIMER 0.35

extern ALLEGRO_COLOR BKG_COLOR;
extern ALLEGRO_FONT *FONT_32;

#define SCREEN_W 1920
#define SCREEN_H 1080

#endif