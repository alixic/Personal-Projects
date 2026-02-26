#include <U8g2lib.h>

#define ECHO_PIN 3
#define TRIG_PIN 4

#define BUZZER_PIN 5
#define BUZZER_FREQUENCY 400

#define PIR_PIN A1
#define PIR_THRESHOLD 600

float distance = 0;

U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 

void setup() {
  Serial.begin(9600);
  pinMode(ECHO_PIN,INPUT);
  pinMode(TRIG_PIN,OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  u8g2.begin();

  Serial.println("SETUP COMPLETE!");
}

void loop() {
  // put your main code here, to run repeatedly:
  TickAlgorithm();

}

void TickAlgorithm()
{
  u8g2.clearBuffer();					
  u8g2.setFont(u8g2_font_ncenB08_tr);	
  distance = GetDistance();
  //read value by PIR sensor
  int value = analogRead(PIR_PIN);
  Serial.println("PIR sensor value: " + String(value));

  if(value > PIR_THRESHOLD)
  {
    Serial.println("TRIGGERED PIR SENSOR!");
    tone(BUZZER_PIN , BUZZER_FREQUENCY);

    u8g2.drawStr(0,10,"WARNING");
    u8g2.sendBuffer();

    return;
  }else if(distance > 100)
  {
    noTone(BUZZER_PIN);
  }
  
  
  //assuming no movement by the PIR sensor

  if(distance > 100)
  {
    //distance too great,do nothing
    u8g2.drawStr(0,10,"OK");
    u8g2.sendBuffer();
    return;
  }
  else if(distance < 10)
  {
    //distance too little,beep,display alert
    tone(BUZZER_PIN , BUZZER_FREQUENCY);

    u8g2.drawStr(0,10,"ALERT");
    u8g2.sendBuffer();

    return;
  }
  else 
  {
    int ms = GetTimeToBeep(distance);

    u8g2.setCursor(0,24);
    u8g2.print(distance);
    u8g2.print(" cm");
    u8g2.sendBuffer();

    BuzzForSeconds(ms);
    NoBeep(ms);
    return;
  }

  delay(2000);
}

int GetTimeToBeep(float distance)
{
  int time = (int) ((8900 - 80*distance) / 9);
  return time;
}

void BuzzForSeconds(int milliseconds)
{
  tone(BUZZER_PIN , BUZZER_FREQUENCY , milliseconds);
  Serial.println("Beeping for : " + String(milliseconds) + " milliseconds");
}

void NoBeep(int millisecondsToBeep)
{
  int ms = 1000 - millisecondsToBeep;
  Serial.println("Not beeping for " + String(ms)  + " milliseconds");
  noTone(BUZZER_PIN);
  delay(ms);
  
}


float GetDistance()
{
  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN,LOW);

  float d = pulseIn(ECHO_PIN,HIGH);
  d = (float) d/58;
  Serial.println("Distance is: " + String(d));
  return d;
}