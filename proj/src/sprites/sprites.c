#include <lcom/lcf.h>

#include "sprites.h"
#include "../controller/graphics.h"

Sprite *mouseCursor;
Sprite *letterSprites[26] = { NULL };
Sprite *youWonSprite;
Sprite *youLoseSprite;

Sprite *create_sprite(xpm_map_t pic) {
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    if( sp == NULL ) return NULL;

    sp->map = (uint32_t *) xpm_load(pic, XPM_8_8_8_8, &img);

    if( sp->map == NULL ) {
        printf("The sprite is null\n");
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

    //letters
    letterSprites[0] = create_sprite((xpm_map_t) a_xpm);
    letterSprites[1] = create_sprite((xpm_map_t) b_xpm);
    letterSprites[2] = create_sprite((xpm_map_t) c_xpm);
    letterSprites[3] = create_sprite((xpm_map_t) d_xpm);
    letterSprites[4] = create_sprite((xpm_map_t) e_xpm);
    letterSprites[5] = create_sprite((xpm_map_t) f_xpm);
    letterSprites[6] = create_sprite((xpm_map_t) g_xpm);
    letterSprites[7] = create_sprite((xpm_map_t) h_xpm);
    letterSprites[8] = create_sprite((xpm_map_t) i_xpm);
    letterSprites[9] = create_sprite((xpm_map_t) j_xpm);
    letterSprites[10] = create_sprite((xpm_map_t) k_xpm);
    letterSprites[11] = create_sprite((xpm_map_t) l_xpm);
    letterSprites[12] = create_sprite((xpm_map_t) m_xpm);
    letterSprites[13] = create_sprite((xpm_map_t) n_xpm);
    letterSprites[14] = create_sprite((xpm_map_t) o_xpm);
    letterSprites[15] = create_sprite((xpm_map_t) p_xpm);
    letterSprites[16] = create_sprite((xpm_map_t) q_xpm);
    letterSprites[17] = create_sprite((xpm_map_t) r_xpm);
    letterSprites[18] = create_sprite((xpm_map_t) s_xpm);
    letterSprites[19] = create_sprite((xpm_map_t) t_xpm);
    letterSprites[20] = create_sprite((xpm_map_t) u_xpm);
    letterSprites[21] = create_sprite((xpm_map_t) v_xpm);
    letterSprites[22] = create_sprite((xpm_map_t) w_xpm);
    letterSprites[23] = create_sprite((xpm_map_t) x_xpm);
    letterSprites[24] = create_sprite((xpm_map_t) y_xpm);
    letterSprites[25] = create_sprite((xpm_map_t) z_xpm);

}

void destroySprites(){
    //Destroy the loaded sprites
    destroy_sprite(mouseCursor);


    for (int i = 0; i < 26; i++) {
        destroy_sprite(letterSprites[i]);
    }

    destroy_sprite(youWonSprite);
    destroy_sprite(youLoseSprite);
}

int drawText(const char *text, int x, int y){
    int offset = 0;
    for(const char *p = text; *p != '\0'; p++){
        if(*p >= 'A' && *p <= 'Z'){
            int index = *p - 'A';
            drawSprite(letterSprites[index], x + offset, y);
            offset += letterSprites[index]->width;
        } else if(*p == ' '){
            offset += letterSprites[0]->width;
        }
    }

    return 0;
}
