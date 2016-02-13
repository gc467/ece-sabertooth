#include <stdlib.h>
#include <stdio.h>

#define CRC_MASK    0b01111111

void writeBytes(unsigned char cmd, unsigned char addr, unsigned char val){
    //Generate checksum
    unsigned char checksum = ( (cmd + addr + val) & CRC_MASK);
    //putchar(CRC_MASK);
    putchar(cmd);
    putchar(addr);
    putchar(val);
    putchar(checksum);
}

int main(){
    unsigned char autobaud = 0xAA;
    putchar(autobaud);
    unsigned char command = 0;
    unsigned char address = 128;
    unsigned char value = 64;
    
    writeBytes(command, address, value);

}
