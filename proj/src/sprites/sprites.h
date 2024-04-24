#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>

#include "../images/mouse.xpm"



typedef struct {
    uint16_t x, y; // current position
    uint16_t width, height; // dimensions
    uint32_t *map;
} Sprite;

Sprite *create_sprite(xpm_map_t pic);
void destroy_sprite(Sprite *sp);
int drawSprite(Sprite *sprite, int x, int y);
void loadSprites();
void destroySprites();


#endif
