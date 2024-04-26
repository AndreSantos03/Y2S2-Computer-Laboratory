#include <lcom/lcf.h>
#include <stdint.h>

#ifdef LAB3
uint32_t sys_calls_counter = 0;
#endif


int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL)
    return 1;
  *lsb = (uint8_t)(val & 0xFF);
  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL)
    return 1;
  *msb = (uint8_t)(val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL)
    return 1;
  uint32_t i;
  int r = sys_inb(port, &i);
  *value = (uint8_t)(i & 0xFF);
  sys_calls_counter++;
  return r;
}
