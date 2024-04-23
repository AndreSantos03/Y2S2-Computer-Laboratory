#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

int vbe_set_mode(uint16_t mode);
int video_init(uint16_t mode);
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int draw_horizontal_line(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int draw_pixel(uint16_t x, uint16_t y, uint32_t color);
void normalize_color(uint32_t *color);
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);
uint32_t (Green)(unsigned i, uint8_t step, uint32_t first);
uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first);
uint32_t (R)(uint32_t first);
uint32_t (G)(uint32_t first);
uint32_t (B)(uint32_t first);
int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);


#endif /* __GRAPHICS_H */
