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
#include "enemies.h"
#include "heros.h"

int chainEffect(Enemy chain, Enemy checked) {

    float distX = chain.chainX - checked.ship.x;
    float distY = chain.chainY - checked.ship.y;
    float totalDist = distX * distX + distY * distY;
    float minDist = chain.chainRadius * chain.chainScaler + checked.raio;
    return(totalDist <= minDist * minDist);
}
void killEnemy(Enemy *e, Hero *h, int* enemyCount, EnemySprites s, Audio *a) {
    //al_play_sample(s.enemyDeathSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    ALLEGRO_SAMPLE_INSTANCE *sound = a->enemyDeathInstances[a->nextDeathSound];
    al_stop_sample_instance(sound);
    al_set_sample_instance_position(sound, 0);
    al_play_sample_instance(sound);
    ++a->nextDeathSound;
    if(a->nextDeathSound >= 16) {
        a->nextDeathSound = 0;
    }
    e->active = 0;
    e->chainActive = 1;
    e->chainX = e->ship.x;
    e->chainY = e->ship.y;
    e->chainRadius = e->raio * 2.0;
    e->chainTimer = CHAIN_TIMER;
    h->score += 10 * e->raioMultiplier;
    --*enemyCount;
}
int dieCheck(Hero h, Enemy e) {

	float distX = h.ship.x - e.ship.x;
	float distY = h.ship.y - e.ship.y;
	float totalDist = distX * distX + distY * distY;
	float minDist = RAIO_HERO + e.raio;
	return(totalDist <= minDist * minDist);
}
int hitCheck(Tiro t, Enemy e) {

    float distX = t.x - e.ship.x;
    float distY = t.y - e.ship.y;
    float totalDist = distX * distX + distY * distY;
    float minDist = t.raio + e.raio;
    return(totalDist <= minDist * minDist);
}
void increaseEnemies(int waveCount, float* enemyMultiplier) {
    if(waveCount % 2 == 0 && waveCount != 0) {
        *enemyMultiplier += 0.2;
        if(*enemyMultiplier > 4) {
            *enemyMultiplier = 4;
        }
    }
}
void changePattern(int* pattern, int spawnInWaveCount) {
    if(spawnInWaveCount % 10 == 0 && spawnInWaveCount != 0) {
        *pattern = rand() % 6;
    }
}



