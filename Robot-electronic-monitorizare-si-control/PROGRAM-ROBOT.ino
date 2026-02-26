

#include <Servo.h>

Servo myservo;

#define V_SENS 13 // senzor de vibratii
#define laser 11

// PROGRAM ARDUINO NANO
// OBSERVATII : PINUL ECHO ESTE COMUN SI PENTRU ARDUINO NANO SI PENTRU A* ( LA FEL POATE FI SI CEL DE DATA DE LA RECEPTORUL 433 MHz )

// COMPONENTE : 1 - BUTOANE CAPACITIVE ; 2 - SERVO MOTOR ; 3 - SENZOR ULTRASONIC ; 4 - DIODA LASER ;
// 5 - SENZOR PIR ; 6 - BUZZER ; 7 - LED-URI (3 NORMALE + 1 RGB) ; 8 - CONTROL MOTOARE(4 PINI) ; 9 - ECRAN LCD - I2C


// !!!!!!! TREBUIE ADAUGAT SI TRANZISTORUL CARE ALIMENTEAZA PLACA A* LA COMANDA PLACII ARDUINO NANO  !!!!!!!!!!!!!! SAU LEGAM BORNA DE IESIRE A SEMNALULUI LA AMBELE PLACI

int current=0;
int p=3;
int ranr=random(3,4); // RANDOM NUMBER 3 OR 4
int n=0;
int durata=0;
int distanta=0; // valoarea este data in cm


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// XX -- !!!!!! X_X_X_X_x BUTONUL 4 - LEGAT LA PIN-UL A3 ESTE NECESAR IN INTOARCEREA IN FUNCTIA LOOP() - PRACTIC IESIREA DIN ALTE FUNCTII - NU FUNCTIONEAZA


void setup(){

  pinMode(V_SENS,INPUT); // senzor de vibratii - pin 13
  pinMode(A3, INPUT);   // nu exista pana la ultima modificare
  pinMode(8,OUTPUT); // RELEU
  pinMode(9, OUTPUT); // BUZZER
  pinMode(7, OUTPUT); // LED VERDE
  pinMode(6, OUTPUT); // LED GALBEN - DREAPTA INAPOI 
  pinMode(5, OUTPUT); // LED ROSU
  pinMode(4, OUTPUT); // LED RGB - B - DREAPTA - INAINTE
  pinMode(3, OUTPUT); // LED RGB - G - STANGA - ROTIRE INAINTE
  pinMode(2, OUTPUT); // LED RGB - R - DREAPTA - ROTIRE IN SPATE
  pinMode(1, OUTPUT); // trigger pin - tx - 1
  pinMode(0, INPUT);  // echo pin - rx - 0
  pinMode(laser, OUTPUT); // LASER  pin 11
  pinMode(13, INPUT); // pin 13 - senzor de vibratii
  
//  pinMode(S_PIR, INPUT); // SENZOR PIR A6
  //PINUL 12 - COMUNICAREA CU CHIP-UL ATTINY85 - NEIMPLEMENTATA.
  
  digitalWrite(8, HIGH);
  
  delay(100);
  myservo.attach(10);   
                                                                                                                                                      
  
  lcd.init(); // initialize the lcd
  lcd.backlight();


  //
  //
  tone(9,256);
  delay(100);
  noTone(9);
  //
  //  - acest bloc de cod functioneaza - robotul cand porneste emite sunet
  

  delay(100);
  
  digitalWrite(7, HIGH);
  delay(1000);
  digitalWrite(7, LOW);
  delay(500);
  
//  digitalWrite(8, HIGH);
  
  
  ledstr();


  // DECLARAM PE URMA PINII DE LA LED-URI CA INPUT PENTRU A NU INFLUENTA STAREA PINILOR DRIVER-ULUI DE MOTOARE 
}



//void seven_seg(int nr)  {    // O FUNCTIE NOUA - Utilizand ecranul cu 7 segmente putem afisa modul curent de functionare a robotului.  // problema : insuficienta pinilor arduino

  
  //}


  

void configurare_1(){                 
    
//  seven_seg(1);  // - ex : variabila nr definita in functia seven_seg va lua valoarea 1

//
  //if(analogRead(A3)>900) { //iesirea din functie nu functioneaza in alte functii ce au timp mai mare de executie

  // loop();
   //}
//
  
    //PRIMUL FOR
  for(int i=45; i<=170; i++){  // valoarea maxima pe care o poate lua i a fost modificata de la 170 la 150

    myservo.write(i);
    
    delay(8);     // schimbam delay-ul de la valoarea 10 la valoarea 8

    digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    durata=pulseIn(0, HIGH);
    distanta=0.034 * durata / 2;
    
    
    if(distanta<45){      ///////////////    -  valoarea dupa < a fost schimbata de la 40 la 45
      
      myservo.write(i);
      delay(8);
      
      digitalWrite(laser, HIGH);
      tone(9,240);  // modificam acest tone de la valoarea 256 la 240
      delay(50);  // modificam valoarea de la 50 la 90
      noTone(9);
      
      }
    
    if(distanta>40){
      
      myservo.write(i);
      delay(8); // modificam delay-ul de la valoarea 5 la 15
      digitalWrite(laser, LOW);
      
    }
   }



   delay(45); // delay-ul intre rotatia de la stanga la dreapta si cea de la dreapta la stanga sau invers
   distanta=0;
   durata=0; 
   // NOU - reinitializam variabilele distanta si durata 

   
    
    // AL DOILEA FOR
    
   for(int j=165; j>45; j--){   // schimbam variabila de la i la j din for - peste tot in acest for ___ de asemenea am schimbat valoarea initiala a lui j de la 170 la 160
    // MODIFICARE - VAL MIN a lui J a fost modificata de la 1 la 5

    myservo.write(j);
    
    delay(8);       /////////// - schimbam delay-ul de la valoarea 12 la valoarea 10 si acum de la 10 la 8
    
    digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    durata=pulseIn(0, HIGH);
    distanta=0.034 * durata / 2;


    if(distanta<45){     //////////
      
      myservo.write(j);
      delay(8);
      digitalWrite(laser, HIGH);
      tone(9,240);   // modificam acest tone de la valoarea 256 la 240
      delay(50); // modificam acest delay de la valoarea 50 la 90
      noTone(9);
      
      }
   
    if(distanta>40){
      
      myservo.write(j);
      delay(8); // modificam delay-ul de la valoarea 10 la 8
      
      digitalWrite(laser, LOW);
      }
    
   }
   configurare_1();
 }

    






void configurare_2() { // robotul ne urmareste in linie dreapta , trebuie sa stam in fata lui - OBS : punem acest mod pe placa A*

    
  // seven_seg(2);

    if(analogRead(A7)>700){
      digitalWrite(8, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(6, HIGH);
      delay(850);
      digitalWrite(3, LOW);
      digitalWrite(6, LOW);
      digitalWrite(8, HIGH);
      }
    
    digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    durata=pulseIn(0, HIGH);
    distanta=0.034 * durata / 2;
    
    delay(60); // am modificat aceasta valoarea de la valoarea 50 la 60
 

    //if(digitalRead(s_pir)>500){
   
      
    if(distanta>56){
        digitalWrite(8, LOW);
        digitalWrite(4, HIGH);
        digitalWrite(3, HIGH);
        }
     delay(30);
   
    if(distanta<45) {
       digitalWrite(8, HIGH);
       digitalWrite(4, !digitalRead(4));
       digitalWrite(3, !digitalRead(3));
       delay(1000);
    }
    
    
    configurare_2();
   }






void configurare_3(){     // ORIENTARE SI DEPLASARE AUTOMATA IN SPATIU !!!!!!!!

   //p=3 (default value)

  // seven_seg(3);

  // if(analogRead(A3)>900) {   // prin acest if putem iesi din functia configurare_3() - existand delay va fi necesar sa tinem mai mult timp buton 3 apasat

 // loop();
 // }


   digitalWrite(8, LOW); //pornire releu
   digitalWrite(4, HIGH);
   digitalWrite(3, HIGH);
   delay(50); // schimbam acest delay de la valoarea 10 la 50


    digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    durata=pulseIn(0, HIGH);
    distanta=0.034 * durata / 2; //(in cm)
    
   if(distanta<40){

    delay(200);
    digitalWrite(4, !digitalRead(4));
    digitalWrite(3, !digitalRead(3));
    delay(1000); // schimbam acest delay de la 1400 la 1000 
    
   // p=p+1;

  //  if(p>4){
   //   p=3;
  //  }
    
 //   digitalWrite(p, HIGH);

    ranr=random(3,4);
    current = ranr;
    digitalWrite(current, HIGH);
    delay(425);     // acest delay este responsabil pentru unghiul de rotatie - prin micsorarea acestui delay, vom micsora si unghiul de rotatie.
  // - initial valoarea acestui delay a fost 625, o vom schimba la 425
  //  digitalWrite(p, LOW);
    digitalWrite(current, LOW);
    delay(100); // schimbam acest delay de la 20 la 100
    current=0; // nu este tocmai necesar
    
    }
  
  configurare_3();
 }






void configurare_4(){  // vibratii - cutremur - alarma la soc mecanic(lovituri)  -  MONITORIZARE

  if(analogRead(A7)>700){
      tone(9,256);
      delay(100);
      noTone(9);


      //AICI AM RAMAS DE ADAUGAT COD
      
      }
  
  if(digitalRead(13)==1) {

    lcd.clear();
    delay(200);
    lcd.setCursor(3,0);
    lcd.print("VIBRATIE");
    lcd.setCursor(4,1);
    lcd.print("DETECTATA");
    
    for(int op=230;op<=256;op++){
      tone(9, op);
      delay(65);
    } 
    
    delay(50);
    digitalWrite(2, HIGH);
    noTone(9); 
    delay(300); 
    digitalWrite(5, HIGH); 
    digitalWrite(6, HIGH); 
    delay(800);
    digitalWrite(5, LOW); 
    delay(300); 
    digitalWrite(6, LOW); 
    digitalWrite(2, LOW);
    delay(400);

    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("ASTEAPTATI...");
    delay(2500); // STABILIZARE SENZOR DE VIBRATII - PENTRU A NU SE CREA O BUCLA
    lcd.clear();
    modconfig(4);
    
    }
  
  
    
  configurare_4();
}






void configurare_5() {  // control cu telecomanda

    
  pinMode(6, INPUT); // LED GALBEN - DREAPTA INAPOI 
  pinMode(4, INPUT); // LED RGB - B - DREAPTA - INAINTE
  pinMode(3, INPUT); // LED RGB - G - STANGA - ROTIRE INAINTE
  pinMode(2, INPUT);    

  
  configurare_5();
}






void blinkf(int times){ // -- led-ul va lumina de n ori - n reprezentand numarul specific modului de functionare

   
  // vom utiliza led-ul galben - pin 6
  
  int str=0;
  while(str<times){
  digitalWrite(6,HIGH);
  delay(700);
  digitalWrite(6, LOW);
  delay(600);
  str++;
  
  }
  delay(400);
  
}



  


void ledstr() { //ledstr
  
  delay(500);
  digitalWrite(5, HIGH);
  delay(400);
  digitalWrite(5, LOW);
  delay(200);
  digitalWrite(6, HIGH);
  delay(400);
  digitalWrite(6, LOW);
  delay(200);
  digitalWrite(7, HIGH);
  delay(400);
  digitalWrite(7, LOW);
  delay(300);
  
  loop();
  } 




void modconfig(int nr){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("- MODUL -");
  lcd.setCursor(7,1);
  String snr=String(nr);
  lcd.print(snr);
  }



void loop() {

digitalWrite(8,HIGH);

lcd.setCursor(2,0);
lcd.print("- ROBOT -");
lcd.setCursor(4,1);
lcd.print("- NARO -"); // MODIFICAT_____initial se numea NALIN (numele vine de la ultima competitie avuta cu acest robot)



if(analogRead(A0)>900){  // A0 - BUTONUL 1
 // digitalWrite(8,HIGH);

  
  modconfig(1);
  blinkf(1);
  
  delay(100);
  digitalWrite(5, HIGH);
  tone(9,110);
      delay(200); /// DELAY-UL A FOST MODIFICAT DE LA 50 LA 200
      noTone(9);  
  delay(500);
  digitalWrite(5, LOW);

  digitalWrite(8, LOW);
  delay(100);
  
  configurare_1();
}




if(analogRead(A1)>900){

  delay(1700);
  if(analogRead(A2)>900){
    modconfig(5);
    blinkf(5);
    digitalWrite(8, LOW);
  configurare_5();
    }
  
  modconfig(2);
  blinkf(2);
  
  delay(100);
  digitalWrite(5, HIGH);
  tone(9,110);
      delay(200);  // delay modificat de la 50 la 200
      noTone(9);
  delay(300);
  digitalWrite(5, LOW);
  

  digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    durata=pulseIn(0, HIGH);
    distanta=0.034 * durata / 2;
    
  while(distanta>25){ // modificam distanta maxima de la 22 la 25
  
    digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    durata=pulseIn(0, HIGH);
    distanta=0.034 * durata / 2;
}
  digitalWrite(8, LOW);
  configurare_2();
}




if(analogRead(A2)>900){
  
  modconfig(3);
  blinkf(3);
  delay(100);
  digitalWrite(5, HIGH);
  tone(9,110);
      delay(200);     // delay modificat de la 50 la 200
      noTone(9);
  delay(300);  // delay modificat de la 500 la 300
  digitalWrite(5, LOW);
  
  configurare_3();
}



if(analogRead(A3)>900){
  modconfig(4);
  blinkf(4);
  configurare_4();
}








}
