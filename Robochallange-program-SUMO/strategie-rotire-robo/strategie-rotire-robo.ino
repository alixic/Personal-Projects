int Ipins[5]={0,1,2,3,4}; // senzori ir
int Opins[4]={10,11,12,13}; // motoare

void setup() {
  // put your setup code here, to run once:
  for(int i=0;i<5;i++){
    pinMode(Ipins[i], INPUT);
  }
  pinMode(A0,INPUT);
  pinMode(A4,INPUT);//senzor linie
  pinMode(A5,INPUT);//senzor linie

  for(int o=0;o<4;o++){
    pinMode(Opins[o], OUTPUT);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
rotstrat1();
  
}


void stopm(){
  analogWrite(10,0);
  analogWrite(11,0);
}


void rotstrat1(){
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    analogWrite(10, 30);
    analogWrite(11, 30);
    rotstrat2();
  }


//idee - rotatie - traiectorie de patrat - sau in mai multe puncte se opreste - rotatie - pe bucati - oprire pe bucati pt 50 ms pt a face citire

void rotstrat2(){ //continuare rotstrat1
  //pasul 1
  
  //if(!digitalRead(0) && !digitalRead(1) && !digitalRead(2) && !digitalRead(3) && !digitalRead(4)){rotstrat1();}

  if((digitalRead(2) && digitalRead(3)) || (digitalRead(2) && digitalRead(4))){


      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      analogWrite(10,30);
      analogWrite(11,30);

    
      while(digitalRead(1) && !digitalRead(2)){
      digitalWrite(13, HIGH);
      analogWrite(10, 0);
      analogWrite(11, 70);
      
      }

    


   
      while(digitalRead(4) && !digitalRead(2)){
      digitalWrite(12, HIGH);
      analogWrite(11, 0);
      analogWrite(10, 70);
      
      

    }


    
  }

  else if(!digitalRead(0) && !digitalRead(1) && !digitalRead(2) && !digitalRead(3) && !digitalRead(4)){
    delay(10);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    analogWrite(10,30);
    analogWrite(11,30);
  }

  
  
  rotstrat2();

}


