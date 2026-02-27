#include <Arduino.h>
#include "xmotionV3.h"

#define SEARCH_SPEED 30
#define COMBAT_SPEED 90

#define LINE_READ_THRESHOLD 500
//below -> WHITE
//above -> BLACK

#define LEFT_LINESENSOR_PIN A5
#define RIGHT_LINESENSOR_PIN A4

#define RM_DIR 13
#define RM_PWM 11
#define LM_DIR 12
#define LM_PWM 10

enum SumoState  
{
  Search,
  Combat
};
XMotionClass motion;

byte testSpeed = 10;
SumoState state = SumoState::Search;

//sensor readings
int leftIR = 0;
int rightIR = 0;
int frontLeftIR = 0;
int frontMidIR = 0;
int frontRightIR = 0;

int leftLineRead = 1000;
int rightLineRead = 1000;

//function declarations
int LeftIR();
int RightIR();
int FrontRightIR();
int FrontLeftIR();
int FrontMidIR();

void ReadIRSensorInfo();
void ReadLineSensorInfo();

bool SeesWhite();
bool SeesAnything();

bool EnemyIsRight();
bool EnemyIsLeft();
bool EnemyIsFront();

void SearchForEnemies();

void TurnAroundRight(int RightSpeed , int time);

void TurnForwardRight(int RightSpeed , int time);
void TurnBackRight(int Speed,int time);

void RushB(int RushBSpeed,int time);

void Turn180(int speed = 5 , float radius = 0.40);

void setup() {

  pinMode(0, INPUT); // - ir stanga sasiu
  pinMode(1, INPUT); // - ir stanga frontal
  pinMode(2, INPUT); // - ir fata frontal
  pinMode(3, INPUT); // - ir dreapta sasiu
  pinMode(4, INPUT); // - ir frontal dreapta

  pinMode(LEFT_LINESENSOR_PIN,INPUT);
  pinMode(RIGHT_LINESENSOR_PIN,INPUT);

  Serial.begin(9600);
}

void loop() {
  //store IR sensor data

  //TODO:Add remote control thing

  ReadIRSensorInfo();
  ReadLineSensorInfo();

  if(!SeesAnything())
  {
    //while no enemy detected,rotate around like a jerk
    TurnAroundRight(15 , 50);
    return;
  }

  else
  {
    if(EnemyIsFront())
    {
      //Rush B
      //TODO:Modify RUSHB speed

      RushB(40,50);

      return;
    }

    //rotate towards enemy
    else if(EnemyIsRight())
    {
      //rotate right
      motion.StopMotors(1);
      TurnForwardRight(15,1);
      
      return;

    }else if(EnemyIsLeft())
    {
      //rotate left
      motion.StopMotors(1);
      TurnBackRight(15,1);
      return;
    }
  }

}

void ReadIRSensorInfo()
{
  leftIR       = LeftIR();
  rightIR      = RightIR();
  frontLeftIR  = FrontLeftIR();
  frontMidIR   = FrontMidIR();
  frontRightIR = FrontRightIR();

  Serial.println(SeesAnything());
}

void ReadLineSensorInfo()
{
  leftLineRead = analogRead(LEFT_LINESENSOR_PIN);
  rightLineRead = analogRead(RIGHT_LINESENSOR_PIN);
}

#pragma region SensorReadingFunctions

int LeftIR()
{
  return digitalRead(0);
}

int RightIR()
{
  return digitalRead(3);
}

int FrontRightIR()
{
  return digitalRead(4);
}

int FrontLeftIR()
{
  return digitalRead(1);
}

int FrontMidIR()
{
  return digitalRead(2);
}

#pragma endregion

bool SeesWhite()
{
  return (leftLineRead < LINE_READ_THRESHOLD || rightLineRead < LINE_READ_THRESHOLD);
}

bool SeesAnything()
{
    return(leftIR || rightIR || frontLeftIR || frontMidIR || frontRightIR);
}

bool EnemyIsRight()
{
    return (rightIR || frontRightIR);
}

bool EnemyIsLeft()
{
  return(leftIR || frontLeftIR);
}

bool EnemyIsFront()
{
  //return (frontMidIR);
  return ((frontMidIR && frontLeftIR) || (frontMidIR && frontRightIR));
}

void TurnAroundRight(int RightSpeed , int time)
{
  digitalWrite(LM_DIR,LOW);

  RightSpeed = map(RightSpeed,0,100,0,255);
  analogWrite(LM_PWM , RightSpeed);
  analogWrite(RM_PWM,0);
  delay(time);
}

void TurnBackRight(int Speed,int time)
{
  digitalWrite(LM_DIR,LOW);
  digitalWrite(RM_DIR,HIGH);

  Speed = map(Speed,0,100,0,255);

  analogWrite(LM_PWM , Speed);
  analogWrite(RM_PWM , Speed);

  delay(time);
}

void TurnForwardRight(int RightSpeed, int time)
{
  digitalWrite(LM_DIR,HIGH);
  digitalWrite(RM_DIR,LOW);

  RightSpeed = map(RightSpeed,0,100,0,255);
  analogWrite(LM_PWM , RightSpeed);
  analogWrite(RM_PWM,  RightSpeed);
  delay(time);
}

void RushB(int RushBSpeed, int time)
{
  digitalWrite(RM_DIR,HIGH);
  digitalWrite(LM_DIR,HIGH);

  RushBSpeed = map(RushBSpeed,0,100,0,255);

  analogWrite(LM_PWM , RushBSpeed);
  analogWrite(RM_PWM , RushBSpeed);
}

void SearchForEnemies()
{
  if(!SeesWhite())
  {
    motion.Forward(SEARCH_SPEED,0);
    return;
  }

  //full stop
  motion.StopMotors(0);
  motion.Backward(SEARCH_SPEED , 1000);
  motion.StopMotors(0);
  TurnAroundRight(2*SEARCH_SPEED , 300);


  
}

void Turn180(int speed = 5 , float radius = 0.40)
{
  //use with speed=5 and radius 0.4 for about 180 turn :))

  speed = map(speed,0,100,0,255);

  int time = (int) ((PI * radius * 100)/speed);

  time = time * 100;

  motion.ArcTurn(speed,0,time);
}
