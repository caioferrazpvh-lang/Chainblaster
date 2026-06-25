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
#include "heros.h"
#include "ship.h"
#include "tiro.h"
#include "mechanics.h"

//inicialização do herói
void initHero(Hero *h) {

	h->score = 0;
	h->ship.cor = al_map_rgb(20, 20, 180);
	h->ship.x = SCREEN_W/2;
	h->ship.y = SCREEN_H/2;
	h->ship.vel = VELOCIDADE_HEROI;
	h->dir_x = 0;
	h->dir_y = 0;
	h->raio = RAIO_HERO;
	initTiro(&h->ship);
	h->angle = 0;			//giro do tiro
	h->naveHero =al_load_bitmap("images/naveHero.png");
	h->shockwave = al_load_bitmap("images/shockwave.png");
	h->heroDeath[0] = al_load_bitmap("images/explosion1.png");
	h->heroDeath[1] = al_load_bitmap("images/explosion2.png");
	h->heroDeath[2] = al_load_bitmap("images/explosion3.png");
	h->heroDeath[3] = al_load_bitmap("images/explosion4.png");
	h->heroDeath[4] = al_load_bitmap("images/explosion5.png");
	h->heroDeath[5] = al_load_bitmap("images/explosion6.png");
	h->heroDeath[6] = al_load_bitmap("images/explosion7.png");
	h->heroDeath[7] = al_load_bitmap("images/explosion8.png");
}
void drawDeath(Hero h, int frame) {

	int width = al_get_bitmap_width(h.heroDeath[frame]);
	int height = al_get_bitmap_height(h.heroDeath[frame]);
	al_draw_scaled_bitmap(h.heroDeath[frame], 0, 0, width, height, h.ship.x - 100, h.ship.y - 100, 200, 200, 0);
	}

void drawHero(Hero h, int waveCount) {

	
	if(h.naveHero){
	int width = al_get_bitmap_width(h.naveHero);
	int height = al_get_bitmap_height(h.naveHero);
	al_draw_scaled_rotated_bitmap(h.naveHero, width / 2, height / 2, h.ship.x, h.ship.y, 1.0, 1.0, h.angle, 0);
	}
	al_draw_circle(h.ship.x, h.ship.y, RAIO_HERO, al_map_rgb(20, 20, 180), 2);

	al_draw_textf(h.fontePequena, al_map_rgb(105, 210, 250), 30, 20, 0, "Score: %d", h.score);	
	al_draw_textf(h.fontePequena, al_map_rgb(105, 210, 250), 30, 50, 0, "Wave %d", waveCount);	
}

void updateHero(Hero *h) {

	if(h->dir_x != 0 || h->dir_y != 0) {
		h->angle = atan2(h->dir_x, -h->dir_y);
	}
	h->ship.x += h->dir_x * h->ship.vel;
	if(h->ship.x > SCREEN_W - 30) {
		h->ship.x = SCREEN_W - 30;
	}
	if(h->ship.x < 30) {
		h->ship.x = 30;
	}

	h->ship.y += h->dir_y * h->ship.vel;
	 if(h->ship.y > SCREEN_H - 30) {
		h->ship.y = SCREEN_H - 30;
	}
	if(h->ship.y < 30) {
		h->ship.y = 30;
	}
	if(h->ship.tiro.modo != TIRO_ATIVO) {
		h->ship.tiro.x = h->ship.x;
		h->ship.tiro.y = h->ship.y;
	}
	else {
		if(h->ship.tiro.timer > 0)
			h->ship.tiro.timer -= 1.0/FPS;
		else
			initTiro(&h->ship);
	}
}
void gerarTiro(Hero *h, TiroPW t) {
	
	
	if(h->ship.tiro.modo == TIRO_ATIVO) {
	h->ship.tiro.raio += 600 / FPS;
	h->chainRotation += 4 / FPS;
	if(h->ship.tiro.raio > RAIO_TIRO && t.active == 0) {
		h->ship.tiro.raio = RAIO_TIRO;
	}
	int width = al_get_bitmap_width(h->shockwave);
	int height = al_get_bitmap_height(h->shockwave);
	float diameter = h->ship.tiro.raio * 2;
	float scaleX = diameter / width;
	float scaleY = diameter / width;
	//al_draw_scaled_bitmap(h->shockwave, 0, 0, width, height, 
	//h->ship.tiro.x - size / 2, h->ship.tiro.y - size / 2, size,
	//size, 1);

	al_draw_scaled_rotated_bitmap(h->shockwave, width / 2, height / 2, h->ship.tiro.x, h->ship.tiro.y, scaleX , scaleY, h->chainRotation, 0);
	}
	else {

		h->ship.tiro.raio = 1;
		h->chainRotation = 0;
	}
}
void scoreDecrease(Hero *h) {
		
		h->scoreTimer += 1 / FPS;
		if(h->scoreTimer > 1) {
			h->scoreTimer = 0;
			h->score -= 1;
		}
		if(h->score < 0) {
			h->score = 0;
		}
}
void resetHero(Hero *h) {
    h->score = 0;
    h->ship.x = SCREEN_W / 2;
    h->ship.y = SCREEN_H / 2;
    h->ship.vel = VELOCIDADE_HEROI;
    h->dir_x = 0;
    h->dir_y = 0;
    h->angle = 0;
}
//------------------------------------------
//DEFINICAO DE POWERUPS
//------------------------------------------
void drawTiroPW(TiroPW* t) {
	if(t->powerUpShowing) {
		t->scaler = 1.0 + 0.2 * sin(t->timer);
		int width = al_get_bitmap_width(t->tiroPWImage);
		int height = al_get_bitmap_height(t->tiroPWImage);
		al_draw_scaled_rotated_bitmap(t->tiroPWImage, width / 2, height / 2, t->x, t->y, 1 * t->scaler, 1 * t->scaler, 0, 0);
	}
}
void defineTiroPWSpawn(TiroPW* t) {
	if(t->timeIsDefined == 0 && t->active == 0) {
		t->raio = 20;
		t->x = (rand () % 1720) + 100;
		t->y = (rand() % 880) + 100;

		if(t->timeIsDefined == 0) {
			t->timeToSpawn = (rand () % 40) + 10;
			t->timeIsDefined = 1;
		}
	}
}
void spawnTiroPW(TiroPW* t) {
	if(t->timeIsDefined) {
		t->timer += 1 / FPS;
		if(t->timer >= t->timeToSpawn) {
			t->powerUpShowing = 1;
		}
		if(t->timer >= t->timeToSpawn + 15) {
			t->powerUpShowing = 0;
			t->timeIsDefined = 0;
			t->timer = 0;
			t->active = 0;
		}
	}
}
void getTiroPW(TiroPW* t, Hero* h) {
	int distX = h->ship.x - t->x;
	int distY = h->ship.y - t->y;
	int totalDist = sqrt(distX * distX + distY * distY);
	int minDist = t->raio + h->raio;
	if(totalDist <= minDist && t->powerUpShowing == 1) {
		t->powerUpShowing = 0;
		t->timeIsDefined = 0;
		t->active = 1;
		h->ship.tiro.modo = TIRO_INATIVO;
		h->ship.tiro.timer = TEMPO_TIRO;
	}
}
void expireTiroPW(TiroPW* t, Hero* h) {
	if(t->active) {
		t->timeSinceActivated += 1 / FPS;
		if(t->timeSinceActivated >= 10.0) {
				t->active = 0;
				t->timeSinceActivated = 0;
				t->timer = 0;
				//h->ship.tiro.modo = TIRO_INATIVO;
				//h->ship.tiro.timer = 0;
				initTiro(&h->ship);
		}
	}
}
void TiroPWFull(TiroPW* t, Hero* h) {
	defineTiroPWSpawn(t);
	drawTiroPW(t);
	spawnTiroPW(t);
	getTiroPW(t, h);
	expireTiroPW(t, h);
}
