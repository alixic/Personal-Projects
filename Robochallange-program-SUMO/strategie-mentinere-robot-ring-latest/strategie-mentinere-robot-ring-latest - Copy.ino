#include "analogComp.h"


#define Left 1 // left frontal sensor
#define Forward 2 // forward frontal sensor
#define Right 4 // right frontal sensor

#define LeftCHS 0 // left chassis sensor
#define RightCHS 3 // right chassis sensor

#define LineR A4
#define LineL A5 

#define FWspeed 220
//12 - sens motor stanga
//13 - sens motor dreapta

int Ipins[5]={0,1,2,3,4};
int Opins[4]={10,11,12,13};


void ringkeep(){
  analogWrite(10,0);
  analogWrite(11,0);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(5);
  analogWrite(10,240);
  analogWrite(11,240);
  delay(300);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  analogWrite(10,170);
  analogWrite(11,170);
  delay(90); // trb reglat
  digitalWrite(13, HIGH);
  Rstr();
}


void setup() {
  // put your setup code here, to run once:
  for(int i=0;i<5;i++){
    pinMode(Ipins[i], INPUT);
  }

  for(int i=0;i<4;i++){
    pinMode(Opins[i], OUTPUT);
  }

  pinMode(LineR, INPUT);
  pinMode(LineL, INPUT);

  analogComparator.setOn(A4, A5); 
  analogComparator.enableInterrupt(ringkeep, CHANGE);
}


/* - idee miscare - traiectorie sinusoida
for(int i=60,j=40;i>40 || j<60;i--,j++){
  analogWrite(10,i);
  analogWrite(11,i);
  delay(50);
}
*/

void line_forwardChk(){
  if(!digitalRead(0) && !digitalRead(1) && !digitalRead(2) && !digitalRead(3) && !digitalRead(4)){
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    analogWrite(10, 50);
    analogWrite(11, 50);
  }

  if(digitalRead(Forward) && !digitalRead(Right) && !digitalRead(Left)){
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    analogWrite(10, FWspeed);
    analogWrite(11, FWspeed);
    }
}



void Rstr(){

  

  line_forwardChk();
  
  
  // CAZ 1 - CALIBRARE STANGA

  if( (digitalRead(Left) && !digitalRead(Forward) && !digitalRead(Right)) || (digitalRead(Left) && digitalRead(Forward) && !digitalRead(Right)) ){
    

    while(!digitalRead(Forward)){
      analogWrite(10,40);
      analogWrite(11,40);
      digitalWrite(12,LOW);
      digitalWrite(13,HIGH);
      if(digitalRead(Forward) && !digitalRead(Right) && !digitalRead(Left)){
        break;
      }
    }

    while(!digitalRead(Forward) && digitalRead(Left)){
      analogWrite(10,40);
      analogWrite(11,40);
      digitalWrite(12,LOW);
      digitalWrite(13,HIGH);
      if(digitalRead(Forward) && !digitalRead(Right) && !digitalRead(Left)){ // fortare oprire rotatie calibrare
        break;
      }
    }

        //ForwardChk();

  }



  // CAZ 2 - CALIBRARE - DREAPTA

  if((digitalRead(Right) && !digitalRead(Forward) && !digitalRead(Left)) || (digitalRead(Right) && digitalRead(Forward) && !digitalRead(Left))){

    while(!digitalRead(Forward)){
      analogWrite(10,40);
      analogWrite(11,40);
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
      if(digitalRead(Forward) && !digitalRead(Right) && !digitalRead(Left)){
        break;
      }
    }

    while(!digitalRead(Forward) && digitalRead(Right)){
      analogWrite(10,40);
      analogWrite(11,40);
      digitalWrite(12,LOW);
      digitalWrite(13,HIGH);
      if(digitalRead(Forward) && !digitalRead(Right) && !digitalRead(Left)){
        break;
      }
    }

       //ForwardChk();

  }





  //CAZ 3

  if(!digitalRead(Left) && !digitalRead(Right) && !digitalRead(Forward)){  // nu vedem nimic pe cei din fata - tot aici verificam daca avem detectie pe cei montati pe sasiu

    if(!digitalRead(LeftCHS) && !digitalRead(RightCHS)){ // nu exista detectie absolut deloc
      analogWrite(10,0);
      analogWrite(11,0);
    }

      if(digitalRead(LeftCHS)){

        while(!digitalRead(Left) || (!digitalRead(Left) && !digitalRead(Forward))){  // avem citire doar pe senzorul din stanga sasiu si nu avem citire pe senzorul stanga
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          analogWrite(10,40);
          analogWrite(11,40);
          }
        
        analogWrite(10,0);
        analogWrite(11,0);
        
      }



      if(digitalRead(RightCHS)){

        while(!digitalRead(Right) || (!digitalRead(Right) && !digitalRead(Forward))){  // avem citire doar pe senzorul din stanga sasiu si nu avem citire pe senzorul stanga
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          analogWrite(10,40);
          analogWrite(11,40); 
        } 
        
        analogWrite(10,0);
        analogWrite(11,0);
        
      }
  }

  Rstr(); // repetare functie - bucla
}


void loop() {
  
  /*
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  analogWrite(10,40);
  analogWrite(11,40);
  delay(100);
  analogWrite(10,0);
  analogWrite(11,0);
  */

  Rstr();
}
