#include "Arduino.h"
#include "mongoose_inits.h"
#include "um982.h"

#define SerialGPS1 Serial7
const int32_t baudGPS = 460800;
constexpr int gpsBufferSz = 384;   // Need larger buffer to handle INS messages which are 305 bytes
uint8_t GPS1rxbuffer[gpsBufferSz];
uint8_t GPS1txbuffer[gpsBufferSz];


// um982 gps1(&Serial5);
um982 gps1(&SerialGPS1);

void setup()
{
  delay(5000);
  Serial.begin(115200);
  delay(10);
  SerialGPS1.begin(baudGPS);
  SerialGPS1.addMemoryForRead(GPS1rxbuffer, sizeof(GPS1rxbuffer));
  SerialGPS1.addMemoryForWrite(GPS1txbuffer, sizeof(GPS1txbuffer));
  
  //ethernet_init();
  //mongoose_init();
}

void loop() {
  if (gps1.poll()){
    Serial.println("Got retflag");
    Serial.println(gps1.GGA.latitude);
  }
}
