#include "Kangaroo_Driver_Lib.h"

/**********************************************************************************************
 * Function:        uart_setup()
 * Input:           None.
 * Output:          mraa_uart_context used for serial communication.
 * Notes:           Initializes the uart_context for serial communication, setting up
 *                  uart mode and baud rate.
 *********************************************************************************************/

mraa_uart_context uart_setup() {

    mraa_uart_context uart0 = mraa_uart_init(0);
	mraa_uart_set_baudrate(uart0, 9600);
	mraa_uart_set_mode(uart0, 8, MRAA_UART_PARITY_NONE, 1);

	return uart0;
}

/**********************************************************************************************
 *  SEE: http://www.dimensionengineering.com/datasheets/KangarooPacketSerialReference.pdf
 * 	for information on how to set up CRC and for details on packet format.
 * 	The CRC implementation here was copied from the reference document.
 *********************************************************************************************/

/**********************************************************************************************
 * Function:        crc14(const uint8_t* data, size_t length)
 * Input:           data: the buffer to compute the CRC of.
 *                  length: the length of the data.
 * Output:          The CRC
 * Notes:           A 14-bit CRC computed from the address (excluding its high bit), command,
 *                  data length, and data. It is the last component of the serial packet.
 *                  The lower 7 bits are written into the first byte, and the upper 7 bits into
 *                  the second byte.
 *********************************************************************************************/

uint16_t crc14(const uint8_t* data, size_t length) {

	uint16_t crc = 0x3fff;
	size_t i, bit;

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

/**********************************************************************************************
 * Function:        write_kangaroo_command(uint8_t address, uint8_t command, const uint8_t* data,
                                           uint8_t length, uint8_t* buffer)
 * Input:           address: the address of the Kangaroo.
 *                  command: the command number.
 *                  data: the command data.
 *                  length: the number of bytes of the data.
 *                  buffer: the buffer to write into.
 * Output:          How many bytes were written.
 * Notes:           Use this function to write a command into a buffer. The buffer can be
 *                  used to send the command to the Kangaroo using the mraa_uart_write function.
 *********************************************************************************************/

size_t write_kangaroo_command(uint8_t address, uint8_t command, const uint8_t* data,
                              uint8_t length, uint8_t* buffer){

    size_t i;
    uint16_t crc;

    buffer[0] = address;
    buffer[1] = command;
    buffer[2] = length;
    for (i = 0; i < length; i++) {
        buffer[3 + i] = data[i];
    }
    crc = crc14(buffer, length + 3);
    buffer[3 + length] = crc & 0x7f;
    buffer[4 + length] = (crc >> 7) & 0x7f;

    return 5 + length;
}

/**********************************************************************************************
 * Function:        start_channel(uint8_t address, uint8_t channel_name)
 * Input:           address: the address of the Kangaroo.
 *                  channel_name: the name of the channel that will be initialized.
 * Output:          None.
 * Notes:           Starts a channel. The Kangaroo LED will shine brightly for a third of a
 *                  second. You must call this before Units, Home or Move commands will work.
 *********************************************************************************************/

void start_channel(uint8_t address, uint8_t channel_name) {

    fprintf(stdout, "Initializing channel!\n");

    uint8_t buffer[7];
    uint8_t data[2];

    // data for start command
    data[0] = channel_name;         // channel name
    data[1] = 0;                    // flags

    write_kangaroo_command(address, CMD_START, data, 2, buffer);

	mraa_uart_write(uart0, buffer, 7);

	fprintf(stdout, "Channel Initialized!\n");
}
