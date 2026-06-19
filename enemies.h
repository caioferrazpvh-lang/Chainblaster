
#ifndef ENEMIES_H
#define ENEMIES_H
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "ship.h"
#include "tiro.h"
#include "heros.h"

typedef struct Enemy {

	Ship ship;
	float raio;
	float raioMultiplier;
	int active;
	int pattern;
	int pSide;		//padrao do movimento.
	float fRatio;
	float angle;
	int chainActive;
	float chainX;
	float chainY;
	float chainRadius;
	float chainTimer;
	float chainDelay;
	float chainScaler;
	float bonusVel;
	int dirY;
	int dirX;
	ALLEGRO_BITMAP *enemyImage;
} Enemy;

typedef struct EnemySprites {

	ALLEGRO_BITMAP *enemy1;
	ALLEGRO_BITMAP *enemy2;
	ALLEGRO_BITMAP *enemy3;
	ALLEGRO_BITMAP *enemy4;
	ALLEGRO_SAMPLE *enemyDeathSound;

}EnemySprites;

typedef struct Audio {
	ALLEGRO_SAMPLE *enemyDeathSample;
	ALLEGRO_SAMPLE_INSTANCE *enemyDeathInstances[16];
	int nextDeathSound;
	ALLEGRO_SAMPLE *gameMusic1;
	ALLEGRO_SAMPLE *shootingSound;
}Audio;

void initEnemy(Enemy *e, int id, EnemySprites s, int wavePattern, int waveCount);
void updateEnemy(Enemy *e, Hero *h, int* enemyCount, EnemySprites s, Audio* a);
void drawEnemy(Enemy e, ALLEGRO_BITMAP *shockwave);
void resetEnemies(Enemy* e);
void sinMovement(Enemy *e);
void getDirection(Enemy* e, Hero* h);
void diagonalMovement(Enemy *e);
void invertedDiagonalMovement(Enemy *e);

#endif