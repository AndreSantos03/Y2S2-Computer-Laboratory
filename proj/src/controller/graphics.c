#include <lcom/lcf.h>
#include "graphics.h"

vbe_mode_info_t mode_info;
int bytesPerPixel;
int xResolution;
int yResolution;
int bufferSize;

uint8_t *mainBuffer;
uint8_t *secondaryBuffer;

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


    secondaryBuffer = malloc(sizeof(uint8_t)*bufferSize);
    mainBuffer = vm_map_phys(SELF, (void*) mr.mr_base, bufferSize);


    if(mainBuffer==NULL)
        return 1;

    return vbe_set_mode(mode);
}

void swap_buffers() 
{
    memcpy(mainBuffer, secondaryBuffer, bufferSize);
}

void clear_screen(){
    if(secondaryBuffer == NULL)return;
    memset(secondaryBuffer, 0, xResolution*yResolution*bytesPerPixel); 
}

void free_buffers(){
    free(mainBuffer); 
    free(secondaryBuffer);
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

    // Check if the color is black (-16777216)
    if (color == 0xFF000000) // -16777216 in decimal is 0xFF000000 in hex (black)
        return 0; // Pixel is black, no need to draw

    int pixelPosition = x + xResolution * y;
    int memOffset = pixelPosition * bytesPerPixel;

    if(memcpy(&secondaryBuffer[memOffset], &color, bytesPerPixel) == NULL) return 1;

    return 0;
}

void normalize_color(uint32_t *color){
    //normalizar a cor (vai buscar apenas os bits relevantes)
    if(mode_info.BitsPerPixel!= 32)
    {
        *color &= (BIT(mode_info.BitsPerPixel)-1);
    }
}

uint32_t direct_mode(uint32_t j, uint32_t i, uint8_t step, uint32_t first) {
     uint32_t red = ((1 << mode_info.RedMaskSize) - 1) & ((i + j) * step + (first >> mode_info.RedFieldPosition)) % (1 << mode_info.RedMaskSize);
    uint32_t green = ((1 << mode_info.GreenMaskSize) - 1) & ((i + j) * step + (first >> mode_info.GreenFieldPosition)) % (1 << mode_info.GreenMaskSize);
    uint32_t blue = ((1 << mode_info.BlueMaskSize) - 1) & ((i + j) * step + (first >> mode_info.BlueFieldPosition)) % (1 << mode_info.BlueMaskSize);

    return (red << mode_info.RedFieldPosition) | (green << mode_info.GreenFieldPosition) | (blue << mode_info.BlueFieldPosition);
}

int draw_xpm(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t image;

    uint8_t *image_buffer = xpm_load(xpm, XPM_8_8_8_8, &image);

    for (int h = 0 ; h < image.height ; h++) {
        for (int w = 0 ; w < image.width ; w++) {
            if(draw_pixel(x + w, y + h, *image_buffer) != 0) return 1 ;
            image_buffer++; 
        }
    }
    return 0;
}


uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
}

uint32_t (R)(uint32_t first){
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}

