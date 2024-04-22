#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

int (keyboard_unsubscribe_interrupts)();
int (keyboard_read_status)(uint8_t *status);

int (read_keyboard_output)(uint8_t port, uint8_t *output);

void (kbc_ih)();
int (keyboard_write_command)(uint8_t port, uint8_t commandByte);

int (keyboard_restore)();


#endif /* _LCOM_KEYBOARD_H_ */
