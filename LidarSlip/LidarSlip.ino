#include <OSCMessage.h>

#include <OSCBundle.h>
#if defined(CORE_TEENSY)
#include <SLIPEncodedUSBSerial.h>
#else
#include <SLIPEncodedSerial.h>
#endif

#if defined(CORE_TEENSY)
SLIPEncodedUSBSerial SLIPSerial(Serial);
#else
SLIPEncodedSerial SLIPSerial(Serial);
#endif

#define id "/33"

HardwareSerial Serial1(2);  // UART1/Serial1 pins 16,17

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop(){
  //only send data when connected
  while(Serial1.available()>=9)
    {
        if((0x59 == Serial1.read()) && (0x59 == Serial1.read())) //Byte1 & Byte2
        {
            unsigned char t1 = Serial1.read(); //Byte3
            unsigned char t2 = Serial1.read(); //Byte4

            unsigned int distance = ( t2 << 8 ) + t1;
            
            t1 = Serial1.read(); //Byte5
            t2 = Serial1.read(); //Byte6

            unsigned int strength = ( t2 << 8 ) + t1;

            for(int i=0; i<3; i++) 
            { 
                Serial1.read(); ////Byte7,8,9
            }

            SLIPSerial.beginPacket();
            OSCBundle bun;
            bun.add(id).add(distance).add(strength);
            bun.send(SLIPSerial);
            SLIPSerial.endPacket();
        }
    }
}
