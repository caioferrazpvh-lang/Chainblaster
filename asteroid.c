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
#include "asteroid.h"

void drawAsteroid(Asteroid a) {
    if(a.asteroidIsPassing == 1){
    int width = al_get_bitmap_width(a.asteroidImage);
    int height = al_get_bitmap_height(a.asteroidImage);
    al_draw_scaled_rotated_bitmap(a.asteroidImage, width / 2, height / 2, a.x, a.y, 2, 2, a.angle, 0);
    //al_draw_circle(a.x, a.y, RAIO_ASTEROIDE, al_map_rgb(180, 20, 20), 5);
    }
}
void drawAsteroidPath(Asteroid a) {
    if(a.timer >= a.timerToPass - 2.0 && a.asteroidIsPassing == 0) {
        if(a.startingType == 0) {
            al_draw_line(a.initialX, 0.0, a.initialX + a.distanceX, SCREEN_H, al_map_rgb(180, 20, 20), 5);
        }
        else if(a.startingType == 1) {
            al_draw_line(a.initialX, SCREEN_H, a.initialX + a.distanceX, 0.0, al_map_rgb(180, 20, 20), 5);
        }
    }
}
void planAsteroidPath(Asteroid* a) {
    if(a->pathIsPlanned == 0) {
    a->initialX = rand () % 1920;
    int finalX = rand () % 1920;
    a->distanceX = finalX - a->initialX;

    a->timerToPass = rand () % 20 + 5;
    a->startingType = rand () % 2;
    if(a->startingType == 0) {
        a->initialY = 0;
        a->dirY = 1;
        a->angle = atan2(a->distanceX, -SCREEN_H);
    }
    else {
        a->initialY = SCREEN_H;
        a->dirY = -1;
        a->angle = atan2(a->distanceX, SCREEN_H);
    }
    a->y = a->initialY;
    a->x = a->initialX;
    a->velY = SCREEN_H / FPS;
    a->velX = a->distanceX / FPS;

    a->pathIsPlanned = 1; 
    }
}
void passAsteroid(Asteroid *a) {
    if(a->pathIsPlanned == 1) {
        a->timer += 1 / FPS;
        if(a->timer >= a->timerToPass) {
        a->asteroidIsPassing = 1;
        a->x += a->velX;
        a->y += a->dirY * a->velY;
        }
        if(a->timer >= a->timerToPass + 1) {
            a->asteroidIsPassing = 0;
            a->pathIsPlanned = 0;
            a->timer = 0;
        }
    }
}
int asteroidHeroHitCheck(Asteroid a, Hero h) {
    if(a.asteroidIsPassing == 1){
    float distX = a.x - h.ship.x;
    float distY = a.y - h.ship.y;
    float totalDist = sqrt(distX * distX + distY * distY);
    float minDist = RAIO_HERO + RAIO_ASTEROIDE;
    if(totalDist < minDist) {
        return 1;
    }
}
    return 0;
}
