#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>


typedef struct {
    int x, y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    char *map; // the pixmap
} Sprite;

Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed);
void destroy_sprite(Sprite *sp);
int drawSprite(Sprite *sprite, int x, int y);
void loadSprites();
void destroySprites();


#endif
