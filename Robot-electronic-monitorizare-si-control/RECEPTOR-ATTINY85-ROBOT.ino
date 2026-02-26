// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif


// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
RH_ASK driver(2000, 3, 12, 5); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

int vd=0;

void setup()
{

  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);    // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
  ;
#endif

  //Serial.begin(9600);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
  int i;

  // Message with a good checksum received, dump it.
  //driver.printBuffer("Got:", buf, buflen);
   // Serial.print(buflen);
   // Serial.println("");

    /*
    for(int i=0;i<buflen;i++){
      
        Serial.print(i);
        Serial.print(" ");
        Serial.print(buf[i]);
        Serial.print(" ");
        Serial.println("");
      }
      */

    vd=buf[0];
 
  //  Serial.print(vd);
  //  Serial.println("");
    
    if(vd==48){ // INAINTE
      digitalWrite(0, LOW);
      digitalWrite(1, HIGH);
      digitalWrite(2, HIGH);
      digitalWrite(4, LOW);
      delay(200);
    }
    
    if(vd==49){ // DREAPTA
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
      digitalWrite(2, HIGH);
      digitalWrite(4, LOW);
      delay(200);
    }
    
    if(vd==50){ // STANGA
      digitalWrite(0, LOW);
      digitalWrite(1, HIGH);
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
      delay(200);
    }

   
   if(vd!=48 && vd!=49 && vd!=50){
    //vd=0;
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);

      //ADAUGAM SI AICI UN DELAY DE 100 SAU 80 - DOAR LA NIVEL DE IDEE
    }


   // NOU ADAUGAT - IF  - NEIMPLEMENTAT
   if(vd==51) {
      digitalWrite(0, LOW);
      digitalWrite(1, LOW);
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
      delay(50);
   }
   
  }
  

  vd=0;
  for(int i=0;i<buflen;i++){
      buf[i]=0;
     }
   
}
