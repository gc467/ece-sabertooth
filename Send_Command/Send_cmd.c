#include <stdio.h>
#include <stdlib.h>
#include "mraa.h"
#include <unistd.h>

#define CRC_MASK    0b01111111


mraa_uart_context uart;
/**
 * Initializes the uart connection and sets the baud rate to 9600
 * Setup also sends the autobauding character to the sabertooth (0xAA) in order to initialize comms
 */
int setup(){
    uart = mraa_uart_init(0);
    
    if (uart == NULL) {
            fprintf(stderr, "UART failed to setup\n");
            return EXIT_FAILURE;
    }
    mraa_uart_set_baudrate(uart, 9600);

    //You must send an autobauding character to the Sabertooth before sending any commands to the device
    unsigned char autobaud = 0xAA;
    mraa_uart_write(uart, &autobaud, 1);
    printf("Sent Autobaud: %c", autobaud);
}

/**
 * Stops the uart connection and returns EXIT_SUCCESS if successful
 */
int destroy(){
    mraa_uart_stop(uart);
    mraa_deinit();
    return EXIT_SUCCESS;
}

/**
 * This function takes in three bytes: cmd, addr, and val.
 * It generates a checksum as required by the Sabertooth 2x motor controllers.
 * where checksum = (addr + cmd + val) & 0x01111111.
 * It then sends the bytes in the following order: address, command, value, checksum
 */

void send_command( unsigned char cmd, unsigned char addr, unsigned char val){
    //Generate checksum
    unsigned char checksum = ( (cmd + addr + val) & CRC_MASK);
    mraa_uart_write(uart, &cmd, 1);
    mraa_uart_write(uart, &addr, 1);
    mraa_uart_write(uart, &val, 1);
    mraa_uart_write(uart, &checksum, 1);

    printf("Sent bytes [Command: %c, Address:%c, Value:%c, Checksum:%c,]", cmd, addr, val, checksum);
}

/*
 * Main function
 */
int main(){
    //Initialize the uart connection and sends the autobauding character
    setup();
    //Wait for 3 seconds before sending driving signal
    sleep(3);

    //For now, type commands to send here.
    //TODO: Add additional function to format commands
    unsigned int command = 0;
    unsigned int value = 64;
    unsigned int address = 128;
    
    //Send commands
    send_command(command, address, value);
    

    //Close the uart connection
    destroy();
}
