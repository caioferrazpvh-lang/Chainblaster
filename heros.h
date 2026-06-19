#ifndef HEROS_H
#define HEROS_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "ship.h"


typedef struct Hero {

	Ship ship;
	int dir_x;
	int dir_y;
	int score;
	float scoreTimer;
	int scoreBest;
	float raio;
	float chainRotation;
	float angle;
	ALLEGRO_BITMAP *shockwave;
	ALLEGRO_BITMAP *naveHero;
	ALLEGRO_BITMAP *heroDeath[8];
	ALLEGRO_FONT *fontePequena;
} Hero;

typedef struct TiroPW{
	
	int x;
	int y;
	float scaler;
	int active;
	float raio;
	int powerUpShowing;
	float timer;
	float timeToSpawn;
	float timeSinceActivated;
	int timeIsDefined;
	ALLEGRO_BITMAP *tiroPWImage;

}TiroPW;

void initHero(Hero *h);
void drawHero(Hero h, int waveCount);
void updateHero(Hero *h);
void gerarTiro(Hero *h, TiroPW t);
void scoreDecrease(Hero *h);
void drawDeath(Hero h, int frame);
void resetHero(Hero *h);
void drawTiroPW(TiroPW* t);
void defineTiroPWSpawn(TiroPW* t);
void spawnTiroPW(TiroPW* t);
void getTiroPW(TiroPW* t, Hero* h);
void expireTiroPW(TiroPW* t, Hero* h);
void TiroPWFull(TiroPW* t, Hero* h);
   

#endif