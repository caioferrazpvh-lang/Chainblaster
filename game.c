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
#include "game.h"

void drawMenu(gameSprite i) {

    
    int widthB = al_get_bitmap_width(i.Background);
    int heightB = al_get_bitmap_height(i.Background);
    al_draw_scaled_bitmap(i.Background, 0, 0, widthB, heightB, 0, 0, 1920, 1080, 0);
    int widthC = al_get_bitmap_width(i.Title);
    int heightC = al_get_bitmap_height(i.Title);
    al_draw_scaled_bitmap(i.Title, 0, 0, widthC, heightC, SCREEN_W / 2 - 1400 / 2, 50, 1400, 550, 0);
    int widthP = al_get_bitmap_width(i.PressEnter);
    int heightP = al_get_bitmap_height(i.PressEnter);
    al_draw_scaled_rotated_bitmap(i.PressEnter, widthP / 2, heightP / 2, SCREEN_W / 2, 800, 0.5 + 0.1 * i.scale, 0.5 + 0.1 * i.scale, 0, 0);

}
void drawBar(gameSprite i, float barFiller) {
   //---------------------BARRA DE ATAQUE NO CANTO INFERIOR ESQUERDO----------------------------
   
    /*int fillW = al_get_bitmap_width(i.ShotBarInside);
    int fillH = al_get_bitmap_height(i.ShotBarInside);
    al_draw_scaled_bitmap(i.ShotBarInside, 0, 0, fillW, fillH, 20, 945, 200, 185, 0);
    al_draw_scaled_bitmap(i.ShotBarInside, 0, 0, fillW, fillH, 200, 945, 215, 185, 0);
    al_draw_scaled_bitmap(i.ShotBarInside, 0, 0, fillW, fillH, 395, 945, 200, 185, 0);
    int barW = al_get_bitmap_width(i.ShotBarOutside);
    int barH = al_get_bitmap_height(i.ShotBarOutside);
    al_draw_scaled_bitmap(i.ShotBarOutside, 0, 0, barW, barH, 5, 1000, 600, 90, 0);*/

    //----------------------BARRA DE ATAQUE NO MEIO, EM CIMA-------------------------------------
    
    int fillW = al_get_bitmap_width(i.ShotBarInside);
    int fillH = al_get_bitmap_height(i.ShotBarInside);
    al_draw_scaled_bitmap(i.ShotBarInside, 0, 0, fillW, fillH, ((300 - barFiller) / 100) * 6 + 660, -45, 200 * (barFiller / 100), 185, 0);
    //al_draw_scaled_bitmap(i.ShotBarInside, 0, 0, fillW, fillH, 855, -45, 215, 185, 0);
    //al_draw_scaled_bitmap(i.ShotBarInside, 0, 0, fillW, fillH, 1050, -45, 200, 185, 0);
    int barW = al_get_bitmap_width(i.ShotBarOutside);
    int barH = al_get_bitmap_height(i.ShotBarOutside);
    al_draw_scaled_bitmap(i.ShotBarOutside, 0, 0, barW, barH, 660, 10, 600, 90, 0);
}
void gameMusic(Audio a) {
    
}
float letterAnimation(float letterScaler, float letterCase) {

    if(letterCase == 0) {
        letterScaler += 5 / FPS;
        return letterScaler;
    }
    if(letterCase == 1) {
        letterScaler -= 4 / FPS;
        return letterScaler;
    }
    if(letterCase == 2) {
        letterScaler -= 2 / FPS;
        return letterScaler;
    }
    if(letterCase == 3) {
        letterScaler += 2 / FPS;
        return letterScaler;
    }
    if(letterCase == 4) {
        letterScaler += 4 / FPS;
        return letterScaler;
    }
    if(letterCase == 5) {
        letterScaler -= 2 / FPS;
        return letterScaler;
    }
    if(letterCase == 6) {
        letterScaler += 2 / FPS;
        return letterScaler;
    }
    if(letterCase == 7) {
        letterScaler = 1;
        return letterScaler;
    }
    return 0;
}
void barCharge(float* barFillerTimer, float* barFiller) {
    *barFillerTimer += 1.0 / FPS;
    if(*barFillerTimer >= 1) {
        *barFiller += 10;
        *barFillerTimer = 0;
    }
    if(*barFiller >= 300) {
        *barFiller = 300;
    }
}