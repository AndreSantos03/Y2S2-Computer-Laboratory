#include <lcom/lcf.h>

#include "sprites.h"
#include "../controller/graphics.h"

Sprite *mouseCursor;

Sprite *create_sprite(xpm_map_t pic) {
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
        if (draw_pixel(x + w, y + h, sprite->map[w + h * sprite->width])){
            return 1;
        } 
      }
    }
    return 0; 
}

void loadSprites(){
    //Load the loaded sprites
    mouseCursor = create_sprite((xpm_map_t) mouse_xpm);
}

void destroySprites(){
    //Destroy the loaded sprites
    destroy_sprite(mouseCursor);
}
