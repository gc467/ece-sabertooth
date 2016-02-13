#include <stdio.h>
#include <stdlib.h>
#include "mraa.h"

/**
 * Initializes the uart connection and sets the baud rate to 9600
 */
int setup(){
    mraa_uart_context uart;
    uart = mraa_uart_init(0);
    
    if (uart == NULL) {
            fprintf(stderr, "UART failed to setup\n");
            return EXIT_FAILURE;
    }
    mraa_uart_set_baudrate(uart, 9600);

    //You must send an autobauding character to the Sabertooth before sending any commands to the device
    unsigned char autobaud = 0xAA;
    mraa_uart_write(uart, &autobaud, 1);
}

/**
 * Stops the uart connection and returns EXIT_SUCCESS if successful
 */
int destroy(){
    mraa_uart_stop(uart);
    mraa_deinit();
    return EXIT_SUCCESS;
}

int main(){
    setup();
    








    destroy();
}
