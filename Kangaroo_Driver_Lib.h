#ifndef Kangaroo_Driver_Lib_h
#define Kangaroo_Driver_Lib_h

#include "COMMAND_LIST.h"
#include "mraa.h"
#include <unistd.h>

/**********************************************************************************************
 * FUNCTION PROTOTYPES
 *********************************************************************************************/

    mraa_uart_context uart_setup();

    uint16_t crc14(const uint8_t* data, size_t length);

    size_t write_kangaroo_command(uint8_t address, uint8_t command, const uint8_t* data,
                                  uint8_t length, uint8_t* buffer);

    void start_channel(uint8_t address, uint8_t channel_name);

    void home_channel(uint8_t address, uint8_t channel_name);

#endif // Kangaroo_Driver_Lib_h
