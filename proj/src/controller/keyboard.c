#include "keyboard.h"

int hook_id_keyboard=1;
uint8_t scancode;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no) {
    if(bit_no==NULL)return 1;
    *bit_no= BIT(hook_id_keyboard);
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_keyboard);
}

int (keyboard_unsubscribe_interrupts)(){
    return sys_irqrmpolicy(&hook_id_keyboard);
}

int (keyboard_read_status)(uint8_t *status){
    if(util_sys_inb(STATUS_PORT,status)!=0)return 1;
   return 0; 
}

int (read_keyboard_output)(uint8_t port, uint8_t *output){
    uint8_t status;
    for(int counter = 0; counter < MAX_ATTEMPTS; counter++){
        if(keyboard_read_status(&status) != 0){
            printf("Error with retrieving status!\n");
            return 1;
        }
        if(status & ( TIMEOUT_BIT | PARITY_BIT )){
            printf("Error with retrieving status (bits)!\n");
            return 1;
        }
        if(status & MOUSE_BIT){
            printf("Error with mouse input!\n");
            return 1;
        }
        if ( (status & OUT_BIT) == 1) { 
            if(util_sys_inb(port,output) != 0){
                printf("Error with reading data in the address %x!",port);
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    

    return 1;
}

void (kbc_ih)() {
    read_keyboard_output(KBD_OUT_BUF,&scancode);    
}

int (keyboard_write_command)(uint8_t port, uint8_t commandByte){
    uint8_t status;
    for(int counter = 0; counter < MAX_ATTEMPTS; counter++){
        if(keyboard_read_status(&status) != 0){
            return 1;
        }
        if( (status & IN_BIT_FULL) == 0 ) {
            if(sys_outb(port, commandByte) != 0)
                return 1;
            return 0;

        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}


int keyboard_restore() {
    if (keyboard_write_command(KBC_CMD_REG, KBC_CMD_READ) != 0) {
        return 1;
    }

    uint8_t command;
    if (read_keyboard_output(KBC_OUT_BUF, &command) != 0) {
        return 1;
    }

    command = command | ENABLE_INT;


    if (keyboard_write_command(KBC_CMD_REG, KBC_CMD_WRITE) != 0) {
        return 1;
    }
    if (keyboard_write_command(KBC_CMD_WRITE, command) != 0) {
        return 1;
    }

    return 0;
}
