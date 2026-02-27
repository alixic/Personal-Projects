//#include <xmotionV3.h>

#include "analogComp.h"

#define vstrat1 50
#define vstrat2 50

float value=65380; // 10ms

ISR(TIMER1_OVF_vect)                    // interrupt service routine for overflow
{
  TCNT1 = value; // preload timer - reinitializare ?
  if(analogRead(A4)<500){
    //oprire motoare
    analogWrite(10,0);
    analogWrite(11,0);
  }
}


void lineSens5(){
    analogWrite(10,0);
    analogWrite(11,0);
}

void setup() {
  
  /*
  // COMPARATOR SECTION

  analogComparator.setOn(A5, INTERNAL_REFERENCE); 
  analogComparator.enableInterrupt(lineSens5, FALLING); 
  //analogComparator.disableInterrupt();

  // COMPARATOR SECTION
  */

  //TIMER SECTION -- necesar pt strategia 1

  noInterrupts();                       // disable all interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = value;                        // preload timer
  TCCR1B |= (1 << CS10)|(1 << CS12);    // 1024 prescaler 
  TIMSK1 |= (1 << TOIE1);               // enable timer overflow interrupt ISR

   // interrupts();                         // enable all interrupts

  //TIMER SECTION

  pinMode(A0, INPUT); // start - stop

  //A4,A5 - senzori analogici
  // A4 - senzor de linie (sasiu - dreapta) vedere din spate
  // A5 - senzor de linie (sasiu - stanga) vedere din spate

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  //attachInterrupt(analogPinToInterrupt(A4), blink, CHANGE);

  pinMode(0, INPUT); // - ir stanga sasiu
  pinMode(1, INPUT); // - ir stanga frontal
  pinMode(2, INPUT); // - ir fata frontal
  pinMode(3, INPUT); // - ir dreapta sasiu
  pinMode(4, INPUT); // - ir dreapta frontal
  

  pinMode(5, INPUT); // dip switch 1 - selectare strategie 1
  pinMode(6, INPUT); // dip switch 2 - selectare strategie 2
  pinMode(7, INPUT); // dip switch 3 - selectare strategie 3

  pinMode(10, OUTPUT); // - control viteza - motor stanga - PWM
  pinMode(11, OUTPUT); // - control viteza - motor dreapta - PWM

  pinMode(12, OUTPUT); // - sens rotatie - motor stanga
  pinMode(13, OUTPUT); // - sens rotatie - motor dreapta
  //Serial.begin(9600);

  //senzor dreapta sasiu (vedere din spatele robotului) - pin d3
  //senzor stanga sasiu (-!!-) - pin d0
  //senzor fata - perpendicular inainte - d2
  //senzor fata - dreapta - d4
  //senzor fata - stanga - d1

  // - pentru pini 12,13 valoarea high va reprezenta directia de rotatie - FORWARD


 /* while(analogRead(A0)<500){} // bucla infinita - asteapta comanda de start de la telecomanda

  loop(); */

}

bool SeesAnything()
{
  //returns true if any IR sensor detects an object
  return (digitalRead(0) || digitalRead(1) || digitalRead(2) || digitalRead(3) || digitalRead(4));
}

void RotateAround()
{
  digitalWrite(12,HIGH);
  digitalWrite(13,LOW);

  analogWrite(10,50);
  analogWrite(11,100);

  delay(100);
}



void enLS(){  //A5
  // COMPARATOR SECTION

  analogComparator.setOn(A5, INTERNAL_REFERENCE); 
  analogComparator.enableInterrupt(lineSens5, FALLING); 
  //analogComparator.disableInterrupt();

  // COMPARATOR SECTION
}

void enRS(){  //A4
  interrupts();
}





/* - PROGRAM DEBUG

Serial.print(digitalRead(0));
  Serial.print(" ");
  Serial.print(digitalRead(1));
  Serial.print(" ");
  Serial.print(digitalRead(2));
  Serial.print(" ");
  Serial.print(digitalRead(3));
  Serial.print(" ");
  Serial.print(digitalRead(4));
  Serial.print(" ");
  Serial.println("");

*/


void loop() {
  
 // digitalWrite(12, HIGH);
 // digitalWrite(13, HIGH);

 // analogWrite(10, 50);
 //  analogWrite(11, 50);

    /*
    while(analogRead(A0)<0){

    }
    */

    /*if(digitalRead(5)){
      strat1();
    }
    if(digitalRead(6)){
      strat2();
    }
    if(digitalRead(7)){
      strat2();
    }*/

    while(!SeesAnything())
    {
      RotateAround();
    }
    
}

// FUCTIA MOTR - TRB TESTATA EXPERIMENTAL
void motr(char idmotor, int sens, int viteza){  // idmotor - dreapta / stanga / ambele, viteza - 0 - 100, sens - forward / backwards
// sens - 0/1 -> 0 - spate, 1 - fata

  int nviteza=map(viteza,0,100,0,255);

  if(sens==1){
    switch(idmotor){
    case 'l':
      digitalWrite(12,HIGH);
      analogWrite(10, nviteza);
      break;

    case 'r':
      digitalWrite(13,HIGH);
      analogWrite(11, nviteza);
      break;

    case 'b': //ambele motoare
      digitalWrite(12,HIGH);
      digitalWrite(13,HIGH);
      analogWrite(10, nviteza);
      analogWrite(11, nviteza);
      break;

    default: // oprire motoare
      digitalWrite(10, 0);
      digitalWrite(11, 0);
      break;
    }

  }


  if(sens==0){
    switch(idmotor){
    case 'l':
      digitalWrite(12,LOW);
      analogWrite(10, nviteza);
      break;

    case 'r':     
      digitalWrite(13,LOW);
      analogWrite(11, nviteza);
      break;

    case 'b': //ambele motoare
      digitalWrite(12,LOW);
      digitalWrite(13,LOW);
      analogWrite(10, nviteza);
      analogWrite(11, nviteza);
      break;

    default:
      digitalWrite(10, 0);
      digitalWrite(11, 0);
      break;
    }
    
  }
  //digitalWrite(motor,HIGH); // - setam sensul de rotatie al motorului respectiv
  
}


// TREBUIE IMBUNATATITA STRAT1() !!!!!!!!!!!!

void strat1(){
    delay(10); 
    motr('b',1,20);
    // - introducem conditia ca valorea analogica data de senzorii de linie sa fie mai mare decat un prag?

    while(true){
      if(digitalRead(0) && digitalRead(1) && digitalRead(2) && digitalRead(3) && digitalRead(4)){
        analogWrite(10, 0);
        analogWrite(11, 0);
      }

      else{
        for(int i=0;i<5;++i){
          if(digitalRead(i)){
            switch(i){
              case 0:
                motr('r',1,vstrat1);
                motr('l',0,vstrat1);
                delay(200); // trb testat - aflam delay-ul necesar pe cale experimentala 
                if(digitalRead(2)){}
                else{
                  analogWrite(10,0);
                  analogWrite(11,0);
                }
                break;
    
              case 1:
                motr('r',1,vstrat1);
                motr('l',0,vstrat1);
                delay(200); // trb testat - aflam delay-ul necesar pe cale experimentala 
                if(digitalRead(2)){}
                else{
                  analogWrite(10,0);
                  analogWrite(11,0);
                }
                break;
    
              case 2:
                motr('b',1,90); // trb reglata
                break;
    
              case 3:
                motr('r',0,vstrat1);
                motr('l',1,vstrat1);
                delay(200); // trb testat - aflam delay-ul necesar pe cale experimentala 
                if(digitalRead(2)){}
                else{
                  analogWrite(10,0);
                  analogWrite(11,0);
                }
                break;
    
              case 4:
                motr('l',1,vstrat1);
                motr('r',0,vstrat1);
                delay(30); // trb testat - aflam delay-ul necesar pe cale experimentala 
                if(digitalRead(2)){}
                else{
                  analogWrite(10,0);
                  analogWrite(11,0);
                }
                break;
    
              default:
                break;
               }
             }
           }
        }
    
    }
}



void strat2(){
    motr('r',1,vstrat2);
    motr('l',0,vstrat2);
    if(digitalRead(0) || digitalRead(1) || digitalRead(2) || digitalRead(3) || digitalRead(4)){
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);
      motr('b',0,70);
    }

    while(true){
    
      if(digitalRead(1) && digitalRead(2) && digitalRead(4)){
        motr('b',1,vstrat2);
      }
  
      if(!digitalRead(1) && digitalRead(2) && digitalRead(4)){
        motr('l',0,vstrat2);
        delay(40); // - trebuie aflat pe cale experimentala
        motr('b',1,vstrat2);
      }
  
      if(digitalRead(1) && digitalRead(2) && !digitalRead(4)){
        motr('l',1,vstrat2);
        delay(40); // - trebuie aflat pe cale experimentala
        motr('b',1,vstrat2);
      }
  
      if(!digitalRead(1) && !digitalRead(2) && digitalRead(4)){
        motr('r',1,vstrat2);
        delay(90); // - trebuie aflat pe cale experimentala
        motr('b',1,vstrat2);
      }
  
      if(digitalRead(1) && !digitalRead(2) && !digitalRead(4)){
        motr('r',0,vstrat2);
        delay(90); // - trebuie aflat pe cale experimentala
        motr('b',1,vstrat2);
      }

 
    }
}



void strat3(){
  // miscam robotul si dupa il aducem in repaus si asteptam semnale de detectie de la senzori.
    motr("b",1,60);
    delay(400);
    //motr("b",1,0);
    analogWrite(10,0);
    analogWrite(11,0);
    
    while(true){

      if(digitalRead(0) || digitalRead(1) || digitalRead(2) || digitalRead(3) || digitalRead(4)){
        if(!digitalRead(0) && digitalRead(1) && digitalRead(2) && !digitalRead(3) && digitalRead(4)){
          motr("b",1,80);
          }
        if( (!digitalRead(1) && digitalRead(2) && digitalRead(4)) || (!digitalRead(1) && !digitalRead(2) && digitalRead(4)) ){
          //motr("l",1,60);
          while(!digitalRead(1) && !digitalRead(2) && !digitalRead(4)){motr("l",1,60);}
          }
        if( (digitalRead(1) && digitalRead(2) && !digitalRead(4)) || (digitalRead(1) && !digitalRead(2) && !digitalRead(4)) ){
          while(!digitalRead(1) && !digitalRead(2) && !digitalRead(4)){motr("r",1,60);}
          }
        if( digitalRead(3) && !digitalRead(1) && !digitalRead(2) && !digitalRead(4) ){
          while(!digitalRead(1) && !digitalRead(2) && !digitalRead(4)){motr("l",1,60);}
          }
        if( digitalRead(0) && !digitalRead(1) && !digitalRead(2) && !digitalRead(4) ){
          while(!digitalRead(1) && !digitalRead(2) && !digitalRead(4)){motr("r",1,60);}
          }
     }

     //else{}
    }
}


void testfollower(){}