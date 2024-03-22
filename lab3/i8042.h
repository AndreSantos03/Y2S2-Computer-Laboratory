#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KBD_OUT_BUF 0x60
#define KBD_STATUS_REG 0x64

#define KBD_STAT_OUT_BUFFER (1 << 0)
#define KBD_STAT_PARITY_ERROR BIT(7)
#define KBD_STAT_TIMEOUT_ERROR BIT(6)

#define ESC_BREAKCODE 0x81

#define OK 0


#endif _LCOM_I8042_H_
