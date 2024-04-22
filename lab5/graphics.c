#include <lcom/lcf.h>
#include "graphics.h"

vbe_mode_info_t mode_info;
int bytesPerPixel;
int xResolution;
int yResolution;
int bufferSize;

uint8_t *mainBuffer;
uint8_t *secondaryBuffer;

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
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    secondaryBuffer = malloc(sizeof(uint8_t)*bufferSize);
 
    mainBuffer = vm_map_phys(SELF, (void*) mr.mr_base, bufferSize);

    if(mainBuffer==NULL)
        return 1;

    return vbe_set_mode(mode);
}