#include <lcom/lcf.h>

#include "sprites.h"

Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed) {
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    if( sp == NULL ) return NULL;

    sp->map = (uint32_t *) xpm_load(pic, XPM_INDEXED, &img);

    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }

    sp->width = img.width; 
    sp->height=img.height;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL ) return;
    if( sp ->map )  free(sp->map);
    free(sp);
    sp = NULL; 
}

int drawSprite(Sprite *sprite, int x, int y){ 
    for (int h = 0 ; h < sprite->height ; h++) {
      for (int w = 0 ; w < sprite->width ; w++) {
        if (draw_pixel(x + w, y + h, sprite->colors[w + h*width])){
            return 1;
        } 
      }
    }
    return 0; 
}

void loadSprites(){
    //Load the loaded sprites

}

void destroySprites(){
    //Destroy the loaded sprites
}