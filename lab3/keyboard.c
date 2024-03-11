#include <lab3/lcf.h>
#include <stdint.h>
#include <i8042.h>

#define TIMEOUT_LIMIT 5
#define TIMEOUT_WAIT_TIME 1000 //Value to be implemented

uint8_t scancode;

void kbc_ih() {

    uint8_t attempts_counter = 0;
    uint8_t status;

    while(attempts_counter <= TIMEOUT_LIMIT){
        if (util_sys_inb(KBD_STATUS_REG,&status) !=0){
            printf("Failed to read the Keyboard Status Registry!\nAttempt Number: %d",attempts_counter);
        }
        //errors
        if(status &  KBD_STAT_TIMEOUT_ERROR || status & KBD_STAT_PARITY_ERROR){
            printf("Communication error!\nAttempt Number: %d",attempts_counter);
        }
        //Success
        else if(status & KBD_STAT_OUT_BUFFER){
            util_sys_inb(KBD_OUT_BUF, &scancode);
            return
        }
        attempts_counter ++;
        // timer() TO BE IMPLEMENTED WHEN LAB 2 IS FINISHED!!!!!
    }
}