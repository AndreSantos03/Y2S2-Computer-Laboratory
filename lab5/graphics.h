#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

int vbe_set_mode(uint16_t mode);
int video_init(uint16_t mode);

#endif /* __GRAPHICS_H */
