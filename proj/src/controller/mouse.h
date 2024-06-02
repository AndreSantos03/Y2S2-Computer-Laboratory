#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "graphics.h"

void (mouse_update_packet)();
void (sync_mouse)();
int (mouse_subscribe_interrupts)(uint8_t *bit_no);
int (mouse_unsubscribe_interrupts)();
int (mouse_read_status)(uint8_t *status);
int (read_mouse_out)(uint8_t port, uint32_t *output);
int (write_mouse_cmd)(uint8_t port, uint8_t controlWord);
int (write_mouse)(uint8_t command);
void (mouse_ih)();

#endif /* _MOUSE_H_ */
