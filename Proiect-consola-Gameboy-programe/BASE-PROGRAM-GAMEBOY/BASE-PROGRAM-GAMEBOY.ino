#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>


#define TFT_CS 5
#define TFT_DC 32
#define BUZZER 26 
#define LED1 4 // VERDE
#define LED2 2  // GALBEN


// pins 12,13 for buttons

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define PADDLE_HEIGHT 27
#define PADDLE_SPEED 2
#define BALL_SIZE 6

int misses,score=0;
int paddleY = 90;
int ballX = 120, ballY = 60;
int ballSpeedX = 1, ballSpeedY = 1;

void setup() {
  pinMode(12, INPUT_PULLUP); // Button for moving the paddle up
  pinMode(13, INPUT_PULLUP); // Button for moving the paddle down
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  tft.begin();
  tft.setRotation(0);
  
  
  drawMenu();
  delay(1200);
  drawMenu();
  drawMenu();
  drawMenu();
  

  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(3);

  buzzersound(250,2);

  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Misses: ");
  tft.print(misses);

  tft.setCursor(130,0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.print("Touch score: ");
  tft.print(score);

}

void loop() {

  if (digitalRead(12)==0) {
    paddleY = max(0, paddleY - PADDLE_SPEED);
  }
  if (digitalRead(13)==0) {
    paddleY = min(tft.height() - PADDLE_HEIGHT, paddleY + PADDLE_SPEED);
  }

  ballX += ballSpeedX;
  ballY += ballSpeedY;

  if (ballX <= 0 || ballX >= tft.width() - BALL_SIZE) {
    ballSpeedX *= -1; //COLIZIUNEA CU MARGINILE ECRANULUI ???
  }
  if (ballY <= 0 || ballY >= tft.height() - BALL_SIZE) {
    ballSpeedY *= -1;
  }

  if (ballX <= 10 && ballY >= paddleY && ballY <= paddleY + PADDLE_HEIGHT) {  // Ball collision with paddle ?
    ballSpeedX *= -1;
    digitalWrite(LED1, HIGH);
    score++;
    updateScore();
  }




  if (ballX <= 2 && ballY >= paddleY && ballY <= paddleY + PADDLE_HEIGHT) {  // 2 is paddle width
    ballSpeedX *= -1;
   // digitalWrite(LED1, HIGH);
   // updateScore();
   // score++;
  }
  
  if (ballX <= 0) {
    // Missed the ball
    misses++;
    digitalWrite(LED2, HIGH);
    resetGame();
  }


  // -- Only redraw the parts of the screen that have changed
  tft.fillRect(10, paddleY - PADDLE_SPEED, 3, PADDLE_HEIGHT + 2 * PADDLE_SPEED, ILI9341_BLACK);
  tft.fillRect(ballX - abs(ballSpeedX), ballY - abs(ballSpeedY), BALL_SIZE + 2 * abs(ballSpeedX), BALL_SIZE + 2 * abs(ballSpeedY), ILI9341_BLACK);
  tft.fillRect(10, paddleY, 3, PADDLE_HEIGHT, ILI9341_WHITE);
  tft.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, ILI9341_WHITE);
  delay(12);  // pentru aprox 60 de cadre pe secunda

}




void resetGame() {
  // Reset the game when the ball is missed

  delay(150);
  tft.fillScreen(ILI9341_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Misses: ");
  tft.print(misses);


   /*
  tft.setCursor(120,0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.print("Touch score: ");
  tft.print(score);
  */

  ballX = 320 / 2;
  ballY = 240 / 2;
  ballSpeedX = 1;
  ballSpeedY = 1;


  digitalWrite(BUZZER, HIGH);
  delay(250);
  digitalWrite(BUZZER, LOW);
  delay(150);

  digitalWrite(LED2, LOW);
  
}




void drawMenu() {
  // Draw the menu on the screen
  tft.fillScreen(ILI9341_BLACK);
  delay(600);
  tft.setCursor(60, 150); // center of the screen
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(6);
  tft.print("PONG");
  delay(600); // Show the menu for 2 seconds
}



void buzzersound(int time, int nr){
  for(int i=0;i<nr;i++){
  digitalWrite(BUZZER, HIGH);
  delay(time);
  digitalWrite(BUZZER, LOW);
  delay(time);
  }
}



void updateScore(){
  tft.setCursor(152,0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.print(score);
  digitalWrite(LED1, LOW);
}


