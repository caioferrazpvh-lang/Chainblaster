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

ALLEGRO_COLOR BKG_COLOR;
ALLEGRO_FONT *FONT_32;

void initGlobals() {
	BKG_COLOR = al_map_rgb(10, 10, 10);
}

int main(int argc, char **argv){

	srand(time(NULL));

	int i;
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	//Inicializa o modulo allegro que carrega imagens
	al_init_image_addon();
	if(!al_init_image_addon()) {
	fprintf(stderr, "failed to initialize image addon!\n");
	return -1;
}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(32);

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
	//Animacao do game over;
	ALLEGRO_FONT *fonteGrande = al_load_font("fonts/pixelated2.ttf", 400, 0);
	if(fonteGrande == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
	ALLEGRO_FONT *fonteMedia = al_load_font("fonts/pixelated2.ttf", 80, 0);
	if(fonteMedia == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
	ALLEGRO_FONT *fontePequena = al_load_font("fonts/pixelated2.ttf", 50, 0);
	if(fontePequena == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
	


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	//----------------------Inicialização de variáveis pré jogo-------------------

	//inicializa globais
	initGlobals();

	//inicializa o Estados de jogo da struct Estados.
	Estados Estados = {0};
	Estados.Menu = 1;
	Estados.Jogando = 0;
	Estados.Sair = 0;
	Estados.gameOver = 0;

	//Cria o heroi
	Hero player1;
	initHero(&player1);
	player1.fontePequena = fontePequena;

	//cria os inimigos:
	Enemy enemies[80];
	for(int i = 0; i < 80; ++i) {

		enemies[i].chainActive = 0;
		enemies[i].active = 0;
		enemies[i].enemyImage = NULL;
	}
	int enemyCount = 0;
	int spawnInWaveCount = 0;
	float enemyMultiplier = 1;
	int enemyPattern = 1;			  	//padrao de movimentacao dos inimigos.
	int levaAcabou = 1;		
	int waveCount = 0;
	int timesVelIncreased = 0;
	int enemiesInWave = (int)(NUM_ENEMIES * enemyMultiplier);

	//Cria o asteroide.
	Asteroid Asteroide = {0};
	Asteroide.x = 200;
	Asteroide.y = 200;
	Asteroide.angle = 0;
	Asteroide.asteroidImage = al_load_bitmap("images/asteroid.png");
	
	//Configruaçoes de tempo:
	float spawnTimer = 0;
	float spawnInterval = 0.1;
	float scaler = 0;			//Multiplicador escalar da funcao seno do press enter na tela inicial.
	float barFiller = 100;		//Multiplicador que carrega a barra de ataque.
	float barFillerTimer = 0;
	float gameOverTimer = 0;

	//Variáveis referentes a morte do herói.
	float deathCount = 0;
	int frame = 0;

	//Variáveis referentes a guardar a pontuacao.
	int highscore;

	//inicia o temporizador
	al_start_timer(timer);
	int playing = 1;

	//Sprites dos inimigos
	EnemySprites Sprites = {0};
	ALLEGRO_BITMAP *shockwave = al_load_bitmap("images/shockwave.png");
	Sprites.enemy1 = al_load_bitmap("images/enemy1.png");
	Sprites.enemy2 = al_load_bitmap("images/enemy2.png");
	Sprites.enemy3 = al_load_bitmap("images/enemy3.png");
	Sprites.enemy4 = al_load_bitmap("images/enemy4.png");	

	//Powerup
	TiroPW TiroPW;
	TiroPW.tiroPWImage = al_load_bitmap("images/tiroPW.png");
	
	//Imagem do background
	ALLEGRO_BITMAP *gameBackground = al_load_bitmap("images/gameBackground.png");

	//Efeitos sonoros
	Audio Audio = {0};
	int menuSongIsPlaying = 0;
	int gameSongIsPlaying = 0;
	ALLEGRO_SAMPLE_ID menuSongID;
	ALLEGRO_SAMPLE_ID gameSongID;
	Audio.shootingSound = al_load_sample ("sounds/shootingSound.wav");
	Audio.enemyDeathSample = al_load_sample("sounds/enemyDeath.wav");
	for(int i = 0; i < 16; ++i) {
		Audio.enemyDeathInstances[i] = al_create_sample_instance(Audio.enemyDeathSample);
		al_set_sample_instance_gain(Audio.enemyDeathInstances[i], 0.4);
		al_attach_sample_instance_to_mixer(Audio.enemyDeathInstances[i], al_get_default_mixer());
	}
	ALLEGRO_SAMPLE *MenuSong = al_load_sample("sounds/menuSong.wav");
	Audio.gameMusic1 = al_load_sample("sounds/gameMusic1.ogg");

	//Sprites do menu
	gameSprite gameSprite = {0};
	gameSprite.Background = al_load_bitmap("images/nebula.jpg");
	gameSprite.Title = al_load_bitmap("images/title.png");
	gameSprite.PressEnter = al_load_bitmap("images/press.png");
	gameSprite.ShotBarOutside = al_load_bitmap("images/attackBar.png");
	gameSprite.ShotBarInside = al_load_bitmap("images/attackFill.png");

	//"Animacao do game over.
	ALLEGRO_BITMAP *gameoverBG = al_load_bitmap("images/gameoverBG.png");
	float letterScaler = 0;
	float letterCase = 0;

	//------------------------------Início do jogo---------------------------------

	while(playing) {
		
	
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//Desenha o menu.
		if(Estados.Menu == 1) {
			if(menuSongIsPlaying == 0) {
			al_play_sample(MenuSong, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &menuSongID);
			menuSongIsPlaying = 1;
			}
			//Fazer o movimento do "press enter".
			scaler += 0.5 / FPS;
			gameSprite.scale = sin(scaler * M_PI) + 1;

			drawMenu(gameSprite);
			al_flip_display();

			if(ev.type == ALLEGRO_EVENT_KEY_UP) {

				if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
					al_stop_sample(&menuSongID);
					menuSongIsPlaying = 0;
					
					Estados.Menu = 0;
					Estados.Jogando = 1;
				}
			}
			//Procedimento de fechamento do jogo
			if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			Estados.Menu = 0;
			Estados.Jogando = 1;
			playing = 0;
		}
		}
		else if(Estados.Jogando == 1) {

		//Coloca a música do jogo
		if(gameSongIsPlaying == 0) {
		al_play_sample(Audio.gameMusic1, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &gameSongID);
		gameSongIsPlaying = 1;
		}

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {

			//funcoes base a partir do inicio do jogo.
			int gameBGWidth = al_get_bitmap_width(gameBackground);
			int gameBGHeight = al_get_bitmap_height(gameBackground);
			al_draw_bitmap(gameBackground, 0, 0, 0);
			drawBar(gameSprite, barFiller);
			spawnTimer += 1.0 / FPS;
			scoreDecrease(&player1);
			updateHero(&player1);
			drawHero(player1, waveCount);
			gerarTiro(&player1, TiroPW);
			barCharge(&barFillerTimer, &barFiller);
			TiroPWFull(&TiroPW, &player1);
			
			//Funcoes relativas a passagem de Asteroide.
			drawAsteroid(Asteroide);
			planAsteroidPath(&Asteroide);
			passAsteroid(&Asteroide);
			drawAsteroidPath(Asteroide);
			if(asteroidHeroHitCheck(Asteroide, player1)) {
				Estados.Jogando = 0;
				Estados.Morrendo = 1;
				al_stop_sample(&gameSongID);
				gameSongIsPlaying = 0;
			}
	
		//Define a quantidade de inimigos por wave.
		enemiesInWave = (int)(NUM_ENEMIES * enemyMultiplier);

		//Verifica se a leva acabou, e, caso tenha acabado, inicializa novos inimigos.
		if(levaAcabou) {
			if(spawnTimer >= 0.1 && spawnInWaveCount < enemiesInWave) {

				initEnemy(&enemies[spawnInWaveCount], enemyCount, Sprites, enemyPattern, waveCount);
				++enemyCount;
				++spawnInWaveCount;
				changePattern(&enemyPattern, spawnInWaveCount);
				spawnTimer = 0;
				if(spawnInWaveCount == enemiesInWave) {
				levaAcabou = 0;
				}
			}
		}


			//Atualizacão das informações dos inimigos.
			for(int i = 0; i < enemiesInWave; ++i) {
			
				updateEnemy(&enemies[i], &player1, &enemyCount, Sprites, &Audio);
				drawEnemy(enemies[i], shockwave);

				//Verificação de vida do player.
				if(enemies[i].active) {
					if(dieCheck(player1, enemies[i]))
					{
						Estados.Jogando = 0;
						Estados.Morrendo = 1;
						al_stop_sample(&gameSongID);
						gameSongIsPlaying = 0;
					}
				}
				//Primeiro confere se o tiro esta ativo, e depois se o raio do tiro encontra o raio do inimigo.
				if(enemies[i].active && player1.ship.tiro.modo == TIRO_ATIVO && hitCheck(player1.ship.tiro, enemies[i])) {
					
					killEnemy(&enemies[i], &player1, &enemyCount, Sprites, &Audio);
					barFiller += 20;
					if(barFiller > 300) {
						barFiller = 300;
					}
				}
			}
			//Verifica se a leva acabou, realiza as ações necessárias para começar uma nova.
				if(enemyCount == 0 && levaAcabou == 0) {

					levaAcabou = 1;
					spawnInWaveCount = 0;
					++waveCount;
					increaseEnemies(waveCount, &enemyMultiplier);
				}
			
			//Confere se os inimigos estao no raio da chain, e depois mata-os.
			for(int i = 0; i < enemiesInWave; ++i) {

				if(enemies[i].chainActive == 1) {

					for(int j = 0; j < enemiesInWave; ++j) {

						if(enemies[j].active && enemies[j].chainActive == 0 && chainEffect(enemies[i], enemies[j]) && i !=j ) {

							enemies[j].chainActive = 2;
							enemies[j].chainDelay = CHAIN_DELAY;
							barFiller += 10;
							if(barFiller > 300) {
								barFiller = 300;
							}
							}
						}
					}
				}
			
		
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
		}
			
			//pausa o jogo por 3 segundos se o jogador morrer
			if(!playing) {
				Estados.Morrendo = 1;
				Estados.Jogando = 0;
				//al_rest(3);
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
			//se a tecla for o W
				case ALLEGRO_KEY_W:
					player1.dir_y--;
				break;
			
				case ALLEGRO_KEY_S:
					player1.dir_y++;
				break;

				case ALLEGRO_KEY_A:
					player1.dir_x--;
				break;

				case ALLEGRO_KEY_D:
					player1.dir_x++;
				break;	

				case ALLEGRO_KEY_SPACE:
				if(barFiller >= 100) {
					if(player1.ship.tiro.modo == TIRO_INATIVO) {
						player1.ship.tiro.modo = TIRO_ATIVO;
						al_play_sample(Audio.shootingSound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						//player1.ship.tiro.raio = RAIO_TIRO;
						barFiller -= 100;
					}
				break;
			}
		}
		}			

		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			//printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			switch(ev.keyboard.keycode) {
			//se a tecla for o W
				case ALLEGRO_KEY_W:
					player1.dir_y++;
				break;

				case ALLEGRO_KEY_S:
					player1.dir_y--;
				break;

				case ALLEGRO_KEY_A:
					player1.dir_x++;
				break;

				case ALLEGRO_KEY_D:
					player1.dir_x--;
				break;	
		
				/*
				case ALLEGRO_KEY_SPACE:
					if(Hero.tiro.modo == TIRO_HOLDING)
						Hero.tiro.modo = TIRO_ATIVO;
				break;		
				*/		
			}	
		}
	}

	//Define o que acontece enquanto o player esta morrendo.
	else if(Estados.Morrendo == 1 && frame < 8) {
		deathCount += 1 / FPS;
			drawDeath(player1, frame);
			if(deathCount > 0.3) {
				deathCount = 0;
				++frame;
			}
			al_flip_display();
			if(frame == 8) {
				Estados.Morrendo = 0;
				Estados.gameOver = 1;
			}
		}
	//Define o que acontece na tela de game over.
	else if(Estados.gameOver == 1) {
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			Estados.Menu = 0;
			Estados.Jogando = 1;
			playing = 0;
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		FILE *arquivoLeitura = fopen("highscore.txt", "r");
		if(arquivoLeitura != NULL) {
			fscanf(arquivoLeitura, "%d", &highscore);
			fclose(arquivoLeitura);
		}
		else {
			highscore = 0;
		}

		int BGwidth = al_get_bitmap_width(gameoverBG);
		int BGheight = al_get_bitmap_height(gameoverBG);
		al_draw_scaled_bitmap(gameoverBG, 0, 0, BGwidth, BGheight, 0, 0, SCREEN_W, SCREEN_H, 0);
		al_draw_text(fonteGrande, al_map_rgb(180, 18, 18), 300 * letterScaler, 100, 0, "GAME");
		al_draw_text(fonteGrande, al_map_rgb(180, 18, 18), 800 * letterScaler, 350, 0, "OVER");
		al_draw_text(fonteMedia, al_map_rgb(128, 128, 128), 400, 900, 0, "Pressione enter para continuar");
		if(player1.score > highscore || player1.scoreBest == 1) {
			FILE *arquivoEscreve = fopen("highscore.txt", "w");
			fprintf(arquivoEscreve, "%d", player1.score);
			fclose(arquivoEscreve);
			player1.scoreBest = 1;
			al_draw_textf(fonteMedia, al_map_rgb(180, 18, 18), 600, 700, 0, "New Best: %d points", highscore);
		}
		else {
			al_draw_textf(fonteMedia, al_map_rgb(180, 18, 18), 100, 700, 0, "Score: %d points                   Best: %d points", player1.score, highscore);
		}
		letterScaler = letterAnimation(letterScaler, letterCase);
		if(letterScaler > 1 && letterCase == 0) {
			letterCase = 1;
		}
		if(letterScaler < 0.75 && letterCase == 1) {
			letterCase = 2;
		}
		if(letterScaler < 0.5 && letterCase == 2) {
			letterCase = 3;
		}
		if(letterScaler > 0.75 && letterCase == 3) {
			letterCase = 4;
		}
		if(letterScaler > 1 && letterCase == 4) {
			letterCase = 5;
		}
		if(letterScaler < 0.75 && letterCase == 5) {
			letterCase = 6;
		}
		if(letterScaler > 1 && letterCase == 6) {
			letterCase = 7;
		}
		
		al_flip_display();

			//Reseta as variáveis importantes para o inicio de uma nova partida.
			if(ev.type == ALLEGRO_EVENT_KEY_UP) {

				if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {

					Estados.gameOver = 0;
					Estados.Menu = 1;
					al_flush_event_queue(event_queue);
					spawnTimer = 0;
					gameOverTimer = 0;
					levaAcabou = 1;
					barFiller = 100;
					deathCount = 0;
					frame = 0;
					enemyCount = 0;
					letterCase = 0;
					letterScaler = 1;
					player1.scoreBest = 0;
					initTiro(&player1.ship);
					resetHero(&player1);
					for(int i = 0; i < enemiesInWave; ++i) {
						resetEnemies(&enemies[i]);
					}
					spawnInWaveCount = 0;
					enemyCount = 0;
					levaAcabou = 1;
					enemyMultiplier = 1;
					enemiesInWave = (int)(NUM_ENEMIES * enemyMultiplier);
					waveCount = 0;
					Asteroide.asteroidIsPassing = 0;
					Asteroide.pathIsPlanned = 0;
					TiroPW.powerUpShowing = 0;
					TiroPW.timeIsDefined = 0;
					TiroPW.timer = 0;
					TiroPW.active = 0;
				}	
		}
	}
	
	} //fim do while


	
    
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	

		char my_text[100];
		al_clear_to_color(al_map_rgb(0,0,0));
	 	sprintf(my_text, "Pontuação: %d", (int)player1.score);

		al_flip_display();
		//al_rest(3);	
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(shockwave);
	al_destroy_bitmap(player1.naveHero);
	al_destroy_bitmap(player1.shockwave);
	al_destroy_bitmap(gameoverBG);
	al_destroy_bitmap(Sprites.enemy1);
	al_destroy_bitmap(Sprites.enemy2);
	al_destroy_bitmap(Sprites.enemy3);
	al_destroy_bitmap(Sprites.enemy4);
	al_destroy_bitmap(Asteroide.asteroidImage);
	al_destroy_bitmap(TiroPW.tiroPWImage);
	al_destroy_bitmap(gameBackground);
    al_destroy_bitmap(gameSprite.PressEnter);
    al_destroy_bitmap(gameSprite.Background);
    al_destroy_bitmap(gameSprite.ShotBarOutside);
    al_destroy_bitmap(gameSprite.ShotBarInside);
	al_destroy_font(fonteGrande);
	al_destroy_font(fonteMedia);
	al_destroy_font(fontePequena);
	al_destroy_bitmap(gameSprite.Title);
	for(int i = 0; i < 8; ++i) {
	al_destroy_bitmap(player1.heroDeath[i]);
	}
	al_destroy_sample(MenuSong);
	al_destroy_sample(Audio.gameMusic1);
	al_destroy_sample(Audio.enemyDeathSample);
	for(int i = 0; i < 16; ++i) {
	al_destroy_sample_instance(Audio.enemyDeathInstances[i]);
	}
   
	return 0;
}
