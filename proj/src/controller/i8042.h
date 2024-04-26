#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define MAX_ATTEMPTS 10
#define DELAY_US    20000

//Letter break codes
#define BREAK_ESC 0x81
#define BREAK_A 0x9E
#define BREAK_B 0xB0
#define BREAK_C 0xAE
#define BREAK_D 0xA0
#define BREAK_E 0x92
#define BREAK_F 0xA1
#define BREAK_G 0xA2
#define BREAK_H 0xA3
#define BREAK_I 0x97
#define BREAK_J 0xA4
#define BREAK_K 0xA5
#define BREAK_L 0xA6
#define BREAK_M 0xB2
#define BREAK_N 0xB1
#define BREAK_O 0x98
#define BREAK_P 0x99
#define BREAK_Q 0x90
#define BREAK_R 0x93
#define BREAK_S 0x9F
#define BREAK_T 0x94
#define BREAK_U 0x96
#define BREAK_V 0xAF
#define BREAK_W 0x91
#define BREAK_X 0xAD
#define BREAK_Y 0x95
#define BREAK_Z 0xAC


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


#define ACK 0xFA    
#define NACK 0xFE


#endif /* _LCOM_I8042_H_ */
