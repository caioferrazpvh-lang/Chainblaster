#ifndef ASTEROID_H
#define ASTEROID_H
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "config.h"
#include "ship.h"
#include "tiro.h"
#include "enemies.h"
#include "heros.h"
#include "mechanics.h"
#include "game.h"

typedef struct Asteroid {
    float x;
    float y;
    float raio;
    float angle;
    float timerToPass;
    float timer;
    int distanceX;
    int initialX;
    int initialY;
    float velX;
    float velY;
    int dirY;
    int startingType;
    int pathIsPlanned;
    int asteroidIsPassing;
    ALLEGRO_BITMAP *asteroidImage;
} Asteroid;

void drawAsteroid(Asteroid a);
void planAsteroidPath(Asteroid* a);
void passAsteroid(Asteroid *a);
void drawAsteroidPath(Asteroid a);
int asteroidHeroHitCheck(Asteroid a, Hero h);

#endif