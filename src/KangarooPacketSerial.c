#include <stdio.h>
#include <stdlib.h>

#include "COMMAND_LIST.h"
#include "mraa.h"
#include <unistd.h>

// Sample code to send "START" command to Kangaroo

uint16_t crc14(const uint8_t* data, size_t length) {
	uint16_t crc = 0x3fff; size_t i, bit;
	for (i = 0; i < length; i ++){
		crc ^= data[i] & 0x7f;
		for (bit = 0; bit < 7; bit ++){
			if (crc & 1) {
				crc >>= 1; crc ^= 0x22f0;
			}
			else{
				crc >>= 1;
			}
		}
	}
	return crc ^ 0x3fff;
}

int main() {

/* SEE: http://www.dimensionengineering.com/datasheets/KangarooPacketSerialReference.pdf
 * 	for information on how to set up CRC and for details on packet format.
 * 	The CRC implementation here was copied from the reference document.
 */

	mraa_uart_context uart0 = mraa_uart_init(0);
	mraa_uart_set_baudrate(uart0, 9600);
	mraa_uart_set_mode(uart0, 8, MRAA_UART_PARITY_NONE, 1);

// CHANNEL 1
	sleep(3);

	fprintf(stdout, "Sending data!\n");

	uint8_t buffer[7];
	buffer[0] = 128; // address
	buffer[1] = 32; // command start
	buffer[2] = 2; // data length
	buffer[3] = '1'; // channel name
	buffer[4] = 0; // flags

	uint16_t crc = crc14(buffer, 5);
	buffer[5] = crc & 0x7F; // crc low byte
	buffer[6] = (crc >> 7) & 0x7F; // crc high byte

	mraa_uart_write(uart0, buffer, 7);

	fprintf(stdout, "Sent out data!\n");

// CHANNEL 2
	sleep(3);

	fprintf(stdout, "Sending data!\n");

	buffer[0] = 128; // address
	buffer[1] = 32; // command start
	buffer[2] = 2; // data length
	buffer[3] = '2'; // channel name
	buffer[4] = 0; // flags

	crc = crc14(buffer, 5);
	buffer[5] = crc & 0x7F; // crc low byte
	buffer[6] = (crc >> 7) & 0x7F; // crc high byte

	mraa_uart_write(uart0, buffer, 7);

	fprintf(stdout, "Sent out data!\n");


	while (1){


	}

	return 0;
}
