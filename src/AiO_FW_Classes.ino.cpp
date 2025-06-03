#include "Arduino.h"
#include "mongoose_inits.h"
#include "um982.h"

#define SerialGPS1 Serial7

const int32_t baudGPS = 460800;
constexpr int gpsBufferSz = 384;   // Need larger buffer to handle INS messages which are 305 bytes
uint8_t GPS1rxbuffer[gpsBufferSz];
uint8_t GPS1txbuffer[gpsBufferSz];

// UM98x detection variables
uint32_t baudrates[]{
    460800,
    115200,
    921600};
const uint32_t nrBaudrates = sizeof(baudrates) / sizeof(baudrates[0]);

bool usingUM981 = false;
bool usingUM982 = false;

const int tmp_serial_buffer_size = 2048;
uint8_t tmpGPSrxbuffer[tmp_serial_buffer_size]; // Temp serial rx buffer for detecting / configuring the UM982
uint8_t tmpGPStxbuffer[tmp_serial_buffer_size]; // Temp serial tx buffer for detecting / configuring the UM982

#include "checkUM98x.h" // Include the UM98x detection code
// End

um982 gps1(&SerialGPS1);

void myGGAHandler(const GGA_DATA &gga)
{
  Serial.println(gps1.GGA.latitude);
  Serial.println(gps1.GGA.longitude);
}

void setup()
{
  delay(5000);
  Serial.begin(115200);
  delay(10);

  checkUM98x(); // Check for UM981 or UM982 and set baudrate to 460800 if detected

  SerialGPS1.begin(baudGPS);
  SerialGPS1.addMemoryForRead(GPS1rxbuffer, sizeof(GPS1rxbuffer));
  SerialGPS1.addMemoryForWrite(GPS1txbuffer, sizeof(GPS1txbuffer));

  gps1.onGGA(myGGAHandler);
  
  //ethernet_init();
  //mongoose_init();
}

void loop() {
  gps1.poll();
}
