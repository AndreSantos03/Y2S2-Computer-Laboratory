#include <lcom/lcf.h>
#include "graphics.h"

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
