/* 
  ARDUINO   MCP4802
  D13       SCK
  D11       SDI
  D10       CS  
*/
#include <SPI.h>

#define CHANNEL_A 0x00
#define CHANNEL_B 0x80
#define GAIN_1 0x20
#define GAIN_2 0x00
#define MODE_SHUTDOWN 0x00
#define MODE_ACTIVE 0x10

word channelA = (CHANNEL_A | GAIN_2 | MODE_ACTIVE) << 8;

void setup(){
  SPI.begin();
  pinMode(10, OUTPUT);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
}

void loop(){
  word payload;
  for(word x=0; x<256; x++) {
    payload = channelA | ((x << 4) & 0x0FFF);
    // PORTB = 0;
    digitalWrite(10, LOW);
    SPI.transfer(highByte(payload));
    SPI.transfer(lowByte(payload));
    digitalWrite(10, HIGH);
    // PORTB = 255;
  }
}
