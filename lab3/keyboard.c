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

// Function to read the keyboard status register
 int read_kbd_status(uint8_t *status) {
    if (util_sys_inb(KBD_STATUS_REG, (uint32_t *)status) != OK) {
        return 1; // Error reading status register
    }
    return 0; // Success
}
uint8_t write_command_to_KBC(uint8_t port, uint8_t commandByte) {
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts > 0) {
        if (read_KBC_status(&status) != 0) {
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & FULL_IN_BUFFER) == 0) {
            if (sys_outb(port, commandByte) != 0) {
                printf("Error: Could not write commandByte!\n");
                return 1;
            }
            return 0;  // Success, commandByte written
        }

        //TO IMPLEMENT WITH TIMER ONCE ITS FINISHED
        attempts--;
    }

    // If reached here, all attempts failed
    printf("Error: Buffer is full after maximum attempts!\n");
    return 1;

}
static int read_kbd_data(uint8_t *data) {
    if (util_sys_inb(KBD_OUT_BUF, (uint32_t *)data) != OK) {
        printf("Failed to read keyboard data.\n");
        return 1; // Error reading data
    }
    return 0; // Success
}
