#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>

#include "../images/mouse.xpm"
#include "../images/letters/a.xpm"
#include "../images/letters/b.xpm"
#include "../images/letters/c.xpm"
#include "../images/letters/d.xpm"
#include "../images/letters/e.xpm"
#include "../images/letters/f.xpm"
#include "../images/letters/g.xpm"
#include "../images/letters/h.xpm"
#include "../images/letters/i.xpm"
#include "../images/letters/j.xpm"
#include "../images/letters/k.xpm"
#include "../images/letters/l.xpm"
#include "../images/letters/m.xpm"
#include "../images/letters/n.xpm"
#include "../images/letters/o.xpm"
#include "../images/letters/p.xpm"
#include "../images/letters/q.xpm"
#include "../images/letters/r.xpm"
#include "../images/letters/s.xpm"
#include "../images/letters/t.xpm"
#include "../images/letters/u.xpm"
#include "../images/letters/v.xpm"
#include "../images/letters/w.xpm"
#include "../images/letters/x.xpm"
#include "../images/letters/y.xpm"
#include "../images/letters/z.xpm"



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
int draw_letter(char letter,int x, int y);
int drawText(const char *text, int x, int y);

#endif
