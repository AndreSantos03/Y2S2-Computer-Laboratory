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

int get_letter_index(int break_code) {
    switch (break_code) {
        case BREAK_A:
            return 0;
        case BREAK_B:
            return 1;
        case BREAK_C:
            return 2;
        case BREAK_D:
            return 3;
        case BREAK_E:
            return 4;
        case BREAK_F:
            return 5;
        case BREAK_G:
            return 6;
        case BREAK_H:
            return 7;
        case BREAK_I:
            return 8;
        case BREAK_J:
            return 9;
        case BREAK_K:
            return 10;
        case BREAK_L:
            return 11;
        case BREAK_M:
            return 12;
        case BREAK_N:
            return 13;
        case BREAK_O:
            return 14;
        case BREAK_P:
            return 15;
        case BREAK_Q:
            return 16;
        case BREAK_R:
            return 17;
        case BREAK_S:
            return 18;
        case BREAK_T:
            return 19;
        case BREAK_U:
            return 20;
        case BREAK_V:
            return 21;
        case BREAK_W:
            return 22;
        case BREAK_X:
            return 23;
        case BREAK_Y:
            return 24;
        case BREAK_Z:
            return 25;
        default:
            return -1;
    }
}
