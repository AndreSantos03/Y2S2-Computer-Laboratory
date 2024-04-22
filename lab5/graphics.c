#include <lcom/lcf.h>
#include "graphics.h"

vbe_mode_info_t mode_info;
int bytesPerPixel;
int xResolution;
int yResolution;
int bufferSize;

uint8_t *mainBuffer;
// uint8_t *secondaryBuffer;

struct minix_mem_range mr; 



int vbe_set_mode(uint16_t mode)
{
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); 

    reg86.intno = 0x10;
    reg86.ah = FUNCTION_CALL;
    reg86.al = SET_VBE_MODE;
    reg86.bx = mode | BIT(14);

    if(sys_int86(&reg86) != 0)return 1;
    return 0;
}

int video_init(uint16_t mode){

    if(vbe_get_mode_info(mode, &mode_info))return 1;    

    bytesPerPixel = (mode_info.BitsPerPixel +7)/8;

    xResolution =  mode_info.XResolution;
    yResolution = mode_info.YResolution;
    bufferSize = bytesPerPixel*  xResolution * yResolution;


    mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + bufferSize;

    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))
        return 1;


 
    mainBuffer = vm_map_phys(SELF, (void*) mr.mr_base, bufferSize);

    // secondaryBuffer = malloc(sizeof(uint8_t)*bufferSize);

    if(mainBuffer==NULL)
        return 1;

    return vbe_set_mode(mode);
}

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    //clip vertically to fit 
    if( height + y >= yResolution){
        height = yResolution - y;
    }
    for(int i= y;  i < y+ height; i++){
        if(draw_horizontal_line(x, i, width, color))return 1;
    }
    return 0; 
}

int draw_horizontal_line(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    //clip to make sure to fit the screen
    if(xResolution < x + len){
        len = xResolution - x;
    }

    for(int i = x; i< len + x ; i++){
        if(draw_pixel(i, y, color))return 1;
    }

    return 0;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color){

    //checks to see if pixel is outisde the screen
    if(x >= xResolution || y >= yResolution) 
        return 1;

    int pixelPosition = x + xResolution * y;
    int memOffset = pixelPosition * bytesPerPixel;

    if(memcpy(&mainBuffer[memOffset], &color, bytesPerPixel) == NULL) return 1;

    return 0;
}

void normalize_color(uint32_t *color){
    //normalizar a cor (vai buscar apenas os bits relevantes)
    if(mode_info.BitsPerPixel!= 32)
    {
        *color &= (BIT(mode_info.BitsPerPixel)-1);
    }
}
