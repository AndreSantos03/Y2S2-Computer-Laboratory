#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) { //pegar os 8 ultimos bits (LSBs) de um numero com 16 bits.
 if(lsb == NULL) return 1;
 *lsb = val & 0xFF;
 return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {//pegar os 8 primeiros bits (MSBs) de um numero com 16 bits.
 if(msb == NULL) return 1;
 *msb = val >> 8;
 return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {//pegar os 8 ultimos bits (LSBs) de um numero com 32 bits. 
 if(value == NULL) return 1;
 uint32_t val;
 int ret = sys_inb(port, &val);
 *value = val & 0xFF;
 return ret;
  //Como o sys_inb retorna um valor de 32 bits nós com esta função obrigamos o resultado a ser de 8 bits.
}
