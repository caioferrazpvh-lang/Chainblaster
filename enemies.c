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
#include "mechanics.h"
#include "heros.h"

void initEnemy(Enemy *e, int id, EnemySprites s, int wavePattern, int waveCount) {
	
	if(id % 2 == 0) {				//pSide é utilizado na variação de movimentação dos inimigos de função seno.

		e->pSide = 1;

	}
	else {

		e-> pSide = -1;

	}
	e->raioMultiplier = ((rand () % 4) + 8.0) / 10.0;
	e->bonusVel = waveCount * 0.1;
	e->dirY = 0;
	e->dirX = 0;
	e->chainDelay = CHAIN_DELAY;
	e->chainActive = 0;
	e->chainTimer = 0;
	e->pattern = wavePattern;
	e->raio = RAIO_INIMIGO * e->raioMultiplier;
	e->active = 1;
	e->chainScaler = 0;
	e->ship.cor = al_map_rgb(176, 18, 18);
								
	switch(e->pattern){			//Padrões de spawn dos inimigos(não tem informações sobre a movimentação)


		case 0:					//Inicia os inimigos no centro da tela, em baixo.
		e->fRatio = 0;
		e->ship.vel = 3.0 + e->bonusVel;
		e->ship.y = SCREEN_H;
		e->ship.x = SCREEN_W / 2;
		e->enemyImage = s.enemy1;
		break;

		case 1:						//Inicializa os inimigos em lugares aleatórios em cima e em baixo.
		e->enemyImage = s.enemy2;
		e->ship.vel = 3.0 + e->bonusVel;
		if(id % 2 == 0)
		{
			e->ship.y = 0;
			e->ship.x = rand () % 1920;
			break;
		}
			e->ship.y = 1080;
			e->ship.x = rand () % 1920;
			break;
		
		
		case 2:					//inicia os inimigos no meio da tela, ao lado esquerdo.
		e->enemyImage = s.enemy3;
		e->ship.x = 0;
		e->ship.y = SCREEN_H / 2;
		e->ship.vel = 3.0 + e->bonusVel;
		break;

		case 3:					//Inicia metade dos inimigos no canto inferior, e metade no canto superior, ambos na esquerda.
		e->enemyImage = s.enemy3;
		e->ship.x = SCREEN_W;
		e->ship.y = SCREEN_H / 2;
		e->ship.vel = 3.0 + e->bonusVel;
		break;

		case 4:					
		e->ship.vel = 3.0 + e->bonusVel;
		e->enemyImage = s.enemy2;
		if(id % 2 == 0)
		{
			e->ship.x = 0;
			e->ship.y = rand () % 1080;
			break;
		}
		e->ship.x = SCREEN_W;
		e->ship.y = rand () % 1080;
		break;

		case 5:
		e->ship.vel = 3.0 + e->bonusVel;
		e->enemyImage = s.enemy4;
		if(id % 2 == 0) {
			e->ship.x = 0;
		}
		else {
			e->ship.x = 1920;
		}
		e->ship.y = 0;
		break;
	}
}
void updateEnemy(Enemy *e, Hero *h, int* enemyCount, EnemySprites s, Audio *a) {


	//não faz nada se o inimigo estiver inativo.
	if(e->active) {
		float oldX = e->ship.x;			//armazena o X antigo para calcular o angulo
		float oldY = e->ship.y;			//armazena o Y antigo para calcular o angulo

		switch(e->pattern) {
									//Nesse caso, eu precisei de um ratio crescente para função, por isso, a variável fRatio.(cresce 1.08 por frame)
									//fRatio é individual para cada caso de switch, para que cada padrão possa ter um de acordo com a necessidade.
			case 0:
			sinMovement(e);
			break;

			case 1:
			getDirection(e, h);
			break;

			case 2:
			diagonalMovement(e);
			break;

			case 3:
			invertedDiagonalMovement(e);
			break;

			case 4:
			getDirection(e, h);
			break;
			
			case 5:
			if(*enemyCount % 2 == 0 && e->dirX == 0) {
				e->dirX = -1;
			}
			else if(e->dirX == 0) {
				e->dirX = 1;
			}
			diagonalMovement(e);
			break;
		}
		float dx = e->ship.x - oldX;		//Calcula a diferenca entre o X atual e o antigo.
		float dy = e->ship.y - oldY;		//Calcula a diferença entre o Y atual e o antigo.
		e->angle = atan2(dx, -dy);
	}
	if(e->chainActive == 2) {  
		e->chainDelay -= 1 / FPS;
		if(e->chainDelay < 0) {

			killEnemy(e, h, enemyCount, s, a);
			e->chainDelay = CHAIN_DELAY;
		}
	}
	else if(e->chainActive == 1) {

		e->chainScaler += 6 / FPS;
		if(e->chainScaler > 1) {
			e->chainScaler = 1;
		}
		e->chainTimer -= 1 / FPS;
		if(e->chainTimer < 0)
		{
			e->chainActive = 0;		//Eu não mudo o chain timer, pois quando o inimigo nascer e morrer de novo, o chain timer volta a ser 1.
			e->chainScaler = 0;
		}
	}
}
void drawEnemy(Enemy e, ALLEGRO_BITMAP *shockwave) {

	//se o inimigo está inativo, a função não o desenha.
	if(e.active) {

	int width = al_get_bitmap_width(e.enemyImage);
	int height = al_get_bitmap_height(e.enemyImage);
	al_draw_scaled_rotated_bitmap(e.enemyImage, width / 2, height / 2, e.ship.x, e.ship.y, 1.0 * e.raioMultiplier, 1.0 * e.raioMultiplier, e.angle, 0);
	al_draw_circle(e.ship.x, e.ship.y, e.raio , al_map_rgb(200,18,18), 1);
	}
	if(e.chainActive == 1) {

		int width = al_get_bitmap_width(shockwave);
		int height = al_get_bitmap_height(shockwave);
		al_draw_scaled_bitmap(shockwave, 0, 0, width, height, e.ship.x - (e.raio * 2) * e.chainScaler,
		e.ship.y - (e.raio * 2) * e.chainScaler, (e.raio * 4) * e.chainScaler, (e.raio * 4) * e.chainScaler, 0);
	}
}
void resetEnemies(Enemy* e) {
        e->active = 0;
        e->chainActive = 0;
        e->chainTimer = 0;
        e->chainDelay = CHAIN_DELAY;
        e->chainScaler = 0;
}
void sinMovement(Enemy *e) {
	if(e->dirY == 0) {
	e->dirY = -1;
	}
	if(e->ship.y < 0) {
		e->dirY = 1;
	}
	if(e->ship.y > 1080) {
		e->dirY = -1;
	}
	e->fRatio += e->ship.vel;
	e->ship.x = SCREEN_W / 2 + e->pSide * SCREEN_W/4 * (sin((2 * M_PI/SCREEN_H) * e->fRatio));
	e->ship.y += (e->dirY * e->ship.vel);
}
void getDirection(Enemy* e, Hero* h) {
    float dirX = h->ship.x - e->ship.x;
    float dirY = h->ship.y - e->ship.y;
    float norm = sqrt(dirX * dirX + dirY * dirY);
    e->ship.x += (dirX / norm) * e->ship.vel * 1.4; 
    e->ship.y += (dirY / norm) * e->ship.vel * 1.4; 
}
void diagonalMovement(Enemy *e) {
	if(e->dirY == 0) {
	e->dirY = -1;
	}
	if(e->dirX == 0) {
	e->dirX = 1;
	}
	if(e->ship.x > 1920) {
		e->dirX = -1;
	}
	if(e->ship.x < 0) {
		e->dirX = 1;
	}
	if(e->ship.y < 0) {
		e->dirY = 1;
	}
	if(e->ship.y > 1080) {
		e->dirY = -1;
	}
	e->ship.y += (220 / FPS * e->dirY) * e->ship.vel;
	e->ship.x += (110 / FPS * e->dirX) * e->ship.vel;
}
void invertedDiagonalMovement(Enemy *e) {
	if(e->dirY == 0) {
	e->dirY = 1;
	}
	if(e->dirX == 0) {
	e->dirX = -1;
	}
	if(e->ship.x > 1920) {
		e->dirX = -1;
	}
	if(e->ship.x < 0) {
		e->dirX = 1;
	}
	if(e->ship.y < 0) {
		e->dirY = 1;
	}
	if(e->ship.y > 1080) {
		e->dirY = -1;
	}
	e->ship.y += (220 / FPS * e->dirY) * e->ship.vel;
	e->ship.x += (110 / FPS * e->dirX) * e->ship.vel;
}

