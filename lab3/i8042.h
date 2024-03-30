#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define MAX_ATTEMPTS 10
#define DELAY_US    20000

#define BREAK_ESC 0x81

#define IRQ_KEYBOARD 1
#define IRQ_REENABLE 0x001
#define IRQ_EXCLUSIVE 0x002

#define STATUS_PORT 0x64
#define KBD_OUT_BUF 0x60

//Status output data
#define PARITY_BIT BIT(7)
#define TIMEOUT_BIT BIT(6)
#define MOUSE_BIT BIT(5)
#define INHIBIT_BIT BIT(4)
//irrelevant bits
#define IN_BIT BIT(1)
#define OUT_BIT BIT(0)

#define MAKE_CODE BIT(7)
#define TWO_BYTES 0xE0

#endif /* _LCOM_I8042_H_ */
