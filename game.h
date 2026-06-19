#ifndef GAME_H
#define GAME_H

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

typedef struct Estados {

    int Menu;
    int Jogando;
    int gameOver;
    int Morrendo;
    int Sair;
}Estados;

typedef struct gameSprite {

    ALLEGRO_BITMAP *Title;
    ALLEGRO_BITMAP *PressEnter;
    ALLEGRO_BITMAP *Background;
    float scale;
    ALLEGRO_BITMAP *ShotBarOutside;
    ALLEGRO_BITMAP *ShotBarInside;
}gameSprite;

void gameMusic(Audio a);
void drawMenu(gameSprite i);
void drawBar(gameSprite i, float barFiller);
float letterAnimation(float letterScaler, float letterCase);
void barCharge(float* barFillerTimer, float* barFiller);
#endif