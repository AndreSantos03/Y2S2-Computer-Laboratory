#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>\
#include <i8042.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

static uint32_t sys_inb_calls = 0;

int (kbd_test_scan)() {
    uint8_t bit_no;
    if (keyboard_subscribe_interrupts(&bit_no) != OK) {
        printf("Failed to subscribe to keyboard interrupts\n");
        return 1;
    }

    uint8_t status;
    uint8_t scancode;
    bool esc_pressed = false;

    while (!esc_pressed) {
        if (read_kbd_status(&status) != OK) {
            printf("Failed to read keyboard status\n");
            return 1;
        }

        // Check if there's data in the output buffer
        if (status & KBD_STAT_OUT_BUFFER) {
            // Read scancode
            if (read_kbd_data(&scancode) != OK) {
                printf("Failed to read scancode\n");
                return 1;
            }

            // Print scancode
            uint8_t bytes[1] = {scancode}; // Assuming size of scancode is 1 byte
            if (kbd_print_scancode(true, 1, bytes) != OK) {
                printf("Failed to print scancode\n");
                return 1;
            }

            if (scancode == ESC_BREAKCODE) {
                esc_pressed = true;
            }
        }

        // Increment sys_inb() calls count
        sys_inb_calls++;

        // TO IMPLEMENT WITH TIMER LATER
    }

    if (keyboard_unsubscribe_interrupts() != OK) {
        printf("Failed to unsubscribe from keyboard interrupts\n");
        return 1;
    }

    if (kbd_print_no_sysinb(sys_inb_calls) != OK) {
        printf("Failed to print number of sys_inb() calls\n");
        return 1;
    }

    return 0; // Success
}



int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
