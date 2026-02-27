#define Left 1 // left frontal sensor
#define Forward 2 // forward frontal sensor
#define Right 4 // right frontal sensor

#define LeftCHS 0 // left chassis sensor
#define RightCHS 3 // right chassis sensor

#define LineR A4
#define LineL A5 

//12 - sens motor stanga
//13 - sens motor dreapta

int Ipins[5]={0,1,2,3,4};
int Opins[4]={10,11,12,13};


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
}


/* - idee miscare - traiectorie sinusoida
for(int i=60,j=40;i>40 || j<60;i--,j++){
  analogWrite(10,i);
  analogWrite(11,i);
  delay(50);
}
*/


void str3(){

  if(digitalRead(Forward) && !digitalRead(Right) && !digitalRead(Left)){
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    analogWrite(10, 150);
    analogWrite(11, 150);
    }
  
  
  if((digitalRead(Left) && digitalRead(Forward)) || digitalRead(Left)){
    analogWrite(10,40);
    analogWrite(11,40);
    while(!digitalRead(Forward)){
      digitalWrite(12,LOW);
      digitalWrite(13,HIGH);
    }
  }

  if((digitalRead(Right) && digitalRead(Forward)) || digitalRead(Right)){
    analogWrite(10,40);
    analogWrite(11,40);
    while(!digitalRead(Forward)){
      digitalWrite(12,HIGH);
      digitalWrite(13,LOW);
    }
  }

  if(!digitalRead(Left) && !digitalRead(Right) && !digitalRead(Forward)){  // nu vedem nimic pe cei din fata - tot aici verificam daca avem pe cei montati pe sasiu

    if(!digitalRead(LeftCHS) && !digitalRead(RightCHS)){ // nu exista detectie absolut deloc
      analogWrite(10,0);
      analogWrite(11,0);
    }

      if(digitalRead(LeftCHS)){

        digitalWrite(12, LOW);
        digitalWrite(13, HIGH);
        analogWrite(10,40);
        analogWrite(11,40);

        while(digitalRead(LeftCHS) && !digitalRead(Left)){  // avem citire doar pe senzorul din stanga sasiu si nu avem citire pe senzorul stanga
          }
        
        analogWrite(10,0);
        analogWrite(11,0);

      }

      if(digitalRead(RightCHS)){

        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);
        analogWrite(10,40);
        analogWrite(11,40);

        while(digitalRead(RightCHS) && !digitalRead(Right)){  // avem citire doar pe senzorul din stanga sasiu si nu avem citire pe senzorul stanga
          }
        
        analogWrite(10,0);
        analogWrite(11,0);

      }
  }

  str3();
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

  str3();
}
