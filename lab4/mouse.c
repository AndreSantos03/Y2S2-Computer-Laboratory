#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "mouse.h"


int hook_id_mouse = 4;        
uint32_t byte;
uint8_t byte_index = 0; 
struct packet mouse_packet;
uint8_t mouse_bytes[3];

void (mouse_update_packet)(){

  for (int i = 0 ; i < 3 ; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }

  mouse_packet.lb = mouse_bytes[0] & MOUSE_LB;
  mouse_packet.mb = mouse_bytes[0] & MOUSE_MB;
  mouse_packet.rb = mouse_bytes[0] & MOUSE_RB;
  mouse_packet.x_ov = mouse_bytes[0] & MOUSE_X_OVERFLOW;
  mouse_packet.y_ov = mouse_bytes[0] & MOUSE_Y_OVERFLOW;
  mouse_packet.delta_x = (mouse_bytes[0] & MOUSE_X_DELTA) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1];
  mouse_packet.delta_y = (mouse_bytes[0] & MOUSE_Y_DELTA) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2];
}

void sync_mouse() {
  if (byte_index == 0 && (byte & FIRST_BYTE)) {
    mouse_bytes[byte_index]= byte;
    byte_index++;
  }
  else if (byte_index > 0) { 
    mouse_bytes[byte_index] = byte;
    byte_index++;
  }
}


int (mouse_subscribe_interrupts)(uint8_t *bit_no)
{
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id_mouse);
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int (mouse_unsubscribe_interrupts)()
{
  return sys_irqrmpolicy(&hook_id_mouse);
}

int (mouse_read_status)(uint8_t *status){
    if(util_sys_inb(STATUS_PORT,status)!=0)return 1;
   return 0; 
}

int (read_mouse_out)(uint8_t port, uint32_t * output){
    uint8_t status;
    
    for(int counter = 0; counter < MAX_ATTEMPTS; counter++){
        if(mouse_read_status(&status) != 0){
            printf("Error with retrieving status!\n");
            return 1;
        }
        if(status & ( TIMEOUT_BIT | PARITY_BIT )){
            printf("Error with retrieving status (bits)!\n");
            return 1;
        }
        if( ! (status & MOUSE_BIT)){ //if its a mouse input this must be ticked
            return 1;
        }
        if ( (status & OUT_BIT) == 1) { 
            if(sys_inb(port,output) != 0){
                printf("Error with reading data in the address %x!",port);
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    

    return 1;
}

int (write_mouse_cmd)(uint8_t port, uint8_t controlWord) {
    uint8_t status;
    for(int counter = 0; counter < MAX_ATTEMPTS; counter++){
        if(mouse_read_status(&status) != 0){
            return 1;
        }
        if( (status & IN_BIT_FULL) == 0 ) {
            if(sys_outb(port, controlWord) != 0)
                return 1;
            return 0;

        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}

int (write_mouse)(uint8_t command) {

  uint8_t response;

    int max_attemps = MAX_ATTEMPTS;
    for(int i= 0; i < max_attemps;i++){
        if (write_mouse_cmd(KBC_CMD_REG, MOUSE_WRITE_BYTE)) return 1;
        if (write_mouse_cmd(KBC_OUT_BUF, command)) return 1;
        tickdelay(micros_to_ticks(DELAY_US));
        if (util_sys_inb(KBC_OUT_BUF, &response)) return 1;
        if (response == ACK) return 0;
    }
  return 1;
}


void (mouse_ih)() {
    read_mouse_out(KBC_CMD_WRITE, &byte);
}

