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
#define IN_BIT_FULL BIT(1)
#define OUT_BIT BIT(0)

#define MAKE_CODE BIT(7)
#define TWO_BYTES 0xE0

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_CMD_READ 0x20
#define KBC_CMD_WRITE 0x60
#define KBC_OUT_BUF 0x60
#define ENABLE_INT      BIT(0)

//mouse 
#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define FIRST_BYTE          BIT(3)
#define MOUSE_X_DELTA       BIT(4)
#define MOUSE_Y_DELTA       BIT(5)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)

#define IRQ_MOUSE 12

#define MOUSE_WRITE_BYTE 0xD4
#define ENABLE_DATA_REP 0xF4
#define DISABLE_DATA_REP 0xF5
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_READ_DATA 0xEB

#define ACK 0xFA    
#define NACK 0xFE


#endif /* _LCOM_I8042_H_ */
