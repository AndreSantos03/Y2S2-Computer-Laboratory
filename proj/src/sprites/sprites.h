#ifndef _SPRITES_H_
#define _SPRITES_H_

/**
 * @file sprites.h
 * @brief Contains function declarations for managing sprites.
 */


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


/**
 * @struct Sprite
 * @brief Represents a sprite image.
 */

typedef struct {
    uint16_t x, y; ///< Position of the sprite.
    uint16_t width, height; ///< Width, Height of the sprite.
    uint32_t *map;          ///< Pixel map of the sprite.
} Sprite;

/**
 * @brief Creates a sprite from an XPM image.
 *
 * This function allocates memory for a sprite structure and loads an XPM image
 * into it.
 *
 * @param pic XPM image of the sprite.
 * @return A pointer to the created sprite, or NULL on failure.
 */
Sprite *create_sprite(xpm_map_t pic);

/**
 * @brief Destroys a sprite.
 *
 * This function frees the memory allocated for a sprite.
 *
 * @param sp Pointer to the sprite to destroy.
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Draws a sprite at the specified position.
 *
 * This function draws a sprite at the specified position on the screen.
 *
 * @param sprite Pointer to the sprite to draw.
 * @param x X-coordinate of the top-left corner of the sprite.
 * @param y Y-coordinate of the top-left corner of the sprite.
 * @return 0 on success, non-zero if an error occurs.
 */
int drawSprite(Sprite *sprite, int x, int y);

/**
 * @brief Loads all the sprites needed for the game.
 *
 * This function loads all the sprites required for the game, including the
 * mouse cursor and letter sprites.
 */
void loadSprites();

/**
 * @brief Destroys all loaded sprites.
 *
 * This function frees the memory allocated for all loaded sprites.
 */
void destroySprites();

/**
 * @brief Draws text on the screen using letter sprites.
 *
 * This function renders text on the screen using letter sprites. It supports
 * uppercase letters and spaces.
 *
 * @param text The text to render.
 * @param x X-coordinate of the top-left corner of the text.
 * @param y Y-coordinate of the top-left corner of the text.
 * @return 0 on success.
 */
int drawText(const char *text, int x, int y);

#endif
