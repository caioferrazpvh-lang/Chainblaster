#ifndef MECHANICS_H
#define MECHANICS_H

#include "heros.h"
#include "enemies.h"

int dieCheck(Hero h, Enemy e);
int hitCheck(Tiro t, Enemy e);
void killEnemy(Enemy *e, Hero *h, int* enemyCount, EnemySprites s, Audio *a);
int chainEffect(Enemy chain, Enemy checked);
void increaseEnemies(int waveCount, float* enemyMultiplier);
void changePattern(int* pattern, int spawnInWaveCount);

#endif