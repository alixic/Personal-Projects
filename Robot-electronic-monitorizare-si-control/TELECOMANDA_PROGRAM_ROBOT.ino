

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

#define F_BUTTON 4
#define R_BUTTON 3
#define L_BUTTON 2


RH_ASK driver(2000, 3, 12, 5);


// cod telecomanda arduino nano

void setup() {

#ifdef RH_HAVE_SERIAL
    Serial.begin(9600);    
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
  ;
#endif
  
  Serial.begin(9600);

  pinMode(F_BUTTON, INPUT);
  pinMode(R_BUTTON, INPUT);
  pinMode(L_BUTTON, INPUT);
}

void loop() {

 
 //  delay(100);
   
  // Serial.print(digitalRead(4)); Serial.print(" ");   Serial.print(digitalRead(3)); Serial.print(" ");   Serial.print(digitalRead(2)); Serial.print(" "); Serial.println(""); // TROUBLESHOOTING
  // delay(200);
 //  if(digitalRead(F_BUTTON)==0){ tx_f_b(); } else {}
 //  if(digitalRead(R_BUTTON)==0){ tx_l_b(); } else {}
 //  if(digitalRead(L_BUTTON)==0){ tx_r_b(); } else {}


  /*

  if( ((digitalRead(F_BUTTON)!=0)&&(digitalRead(R_BUTTON)!=0)&&(digitalRead(L_BUTTON)!=0)) ){
      //Serial.println("STOP");
      delay(170);
      tx_stop_b();
    } else{

   if(digitalRead(F_BUTTON)==0){ tx_f_b(); } else {}
   if(digitalRead(R_BUTTON)==0){ tx_l_b(); } else {}
   if(digitalRead(L_BUTTON)==0){ tx_r_b(); } else {}
      
      }

     */

   if( (digitalRead(F_BUTTON)==digitalRead(R_BUTTON)) && (digitalRead(F_BUTTON)==digitalRead(L_BUTTON)) ){  // conditia pentru niciun buton apasat
     delay(10);
     tx_stop_b();
    } else{

   if(digitalRead(F_BUTTON)==0){ tx_f_b(); pinMode(F_BUTTON, INPUT);  } else { pinMode(F_BUTTON, INPUT);
     pinMode(R_BUTTON, INPUT);
     pinMode(L_BUTTON, INPUT); }
     
   if(digitalRead(R_BUTTON)==0){ tx_l_b(); pinMode(R_BUTTON, INPUT); } else { pinMode(F_BUTTON, INPUT);
     pinMode(R_BUTTON, INPUT);
     pinMode(L_BUTTON, INPUT); }
     
   if(digitalRead(L_BUTTON)==0){ tx_r_b(); pinMode(L_BUTTON, INPUT); } else { pinMode(F_BUTTON, INPUT);
     pinMode(R_BUTTON, INPUT);
     pinMode(L_BUTTON, INPUT); }
      
      }
   
} 


void tx_f_b(){
    const char *f_b = "0"; // inainte
    driver.send((uint8_t *)f_b, strlen(f_b));
    driver.waitPacketSent();
    delay(300);
  }


void tx_l_b(){
    const char *l_b = "1"; // stanga
    driver.send((uint8_t *)l_b, strlen(l_b));
    driver.waitPacketSent();
    delay(300);
  }


void tx_r_b(){
    const char *r_b = "2"; // dreapta
    driver.send((uint8_t *)r_b, strlen(r_b));
    driver.waitPacketSent();
    delay(300);
  }


void tx_stop_b(){
    const char *stop_b = "3"; // inainte
    driver.send((uint8_t *)stop_b, strlen(stop_b));
    driver.waitPacketSent();
    delay(250);
  }
