#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// directive preprocesor:

/*
#define LED_1 
#define LED_2
#define LED_3
*/

#define BUZZER 16

#define BTN_UP 36
#define BTN_LEFT 39
#define BTN_CENTER 35
#define BTN_RIGHT 33
#define BTN_DOWN 27

//ECRAN OLED
//#define OLED_RST ?

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// intrari in decodificatorul 1 bcd-7seg
// #define LED_DISPLAY1_p1
// #define LED_DISPLAY1_p2
// #define LED_DISPLAY1_p3
// #define LED_DISPLAY1_p4

// intrari in decodificatorul 2 bcd-7seg
// #define LED_DISPLAY2_p1
// #define LED_DISPLAY2_p2
// #define LED_DISPLAY2_p3
// #define LED_DISPLAY2_p4


// #define VBAT_SENS 
#define SPI_CSN 15
#define CE_PIN 4
//#define OLED_RST 17

// Obiect ecran OLED (SSD1306)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Obiect nrf24l01
RF24 radio(CE_PIN, SPI_CSN); // CE, CSN
const byte add[6] = "10101";


int btn_pins[5]={36,39,35,33,27};


int menuIndex = 0;
int submenuIndex = 0;
bool inSubmenu = false;

// Menu items
const char* mainMenu[] = {
  "Setari",
  "Masuratori",
  "Conexiune",
  "Info. hardware",
  "Altele"
};


const int mainMenuCount = sizeof(mainMenu) / sizeof(mainMenu[0]);

const char* settingsMenu[] = {
  "Optiune 1", 
  "Optiune 2", 
  "Înapoi"
};

const int settingsCount = sizeof(settingsMenu) / sizeof(settingsMenu[0]);

const char* measurementsMenu[] = {"mmenu1", "mmenu2", "mmenu3", "Inapoi"};
const int measurementsCount = sizeof(measurementsMenu) / sizeof(measurementsMenu[0]);


bool settingsStates[] = {false, false};  // State of "Optiune 1" and "Optiune 2"

const char* mmenu1Data[] = {
  "Temp: 22.5 C",
  "Umiditate: 45%",
  "Presiune: 1013 hPa"
};

const char* mmenu2Data[] = {
  "Tensiune: 3.3V",
  "Curent: 120 mA",
  "Putere: 0.4W"
};


void setup() {

    Serial.begin(115200);

    if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED allocation failed");
        while (1);
    }

    radio.begin();
    pinMode(BUZZER, OUTPUT);

    for(int i=0;i<5;i++){
        pinMode(*(btn_pins+i), INPUT);
    }

   // pinMode(VBAT_SENS, INPUT);

    OLED.clearDisplay();
    OLED.setTextSize(1);
    OLED.setTextColor(SSD1306_WHITE);

    for(int i=0;i<128;i++){
        OLED.drawPixel(i,1,WHITE);
    }

    for(int i=64;i>0;i--){
        OLED.drawPixel(128,i,WHITE);
    }

    for(int i=128;i>0;i--){
        OLED.drawPixel(i,64,WHITE);
    }

    for(int i=64;i>0;i--){
        OLED.drawPixel(1,i,WHITE);
    }

    OLED.setCursor(4,2);     
    OLED.println("Terminal de receptie SMCA-PA");
    OLED.println("");
    OLED.display();


    for(int i=29;i<36;i++){
        OLED.setCursor(i,80);
        OLED.print("*");
        delay(500);
    }    

    drawMenu();
}






void sendData(char data[]){
    radio.openWritingPipe(add);
    radio.stopListening();

    radio.write(&data, sizeof(data));
    delay(1000);
}


void receiveData(){
    radio.openReadingPipe(0, add);
    radio.startListening();

    if (radio.available()){
        char text[200] = {0};
        radio.read(&text, sizeof(text));
        
        //Serial.println(text);
    }
}





void loop() {
    handleInput();
}






void handleInput() {
  // SELECT button
  if (!digitalRead(BTN_CENTER)) {
    if (!inSubmenu) {
      inSubmenu = true;
      submenuIndex = 0;
    } else {
      if (menuIndex == 0) { // Setari
        if (submenuIndex < settingsCount - 1) {
          // Toggle ON/OFF logic
          settingsStates[submenuIndex] = !settingsStates[submenuIndex];
        } else {
          inSubmenu = false;
          submenuIndex = 0;
        }
      } else if (menuIndex == 1) { // Masuratori
        if (submenuIndex == measurementsCount - 1) {
          inSubmenu = false;
          submenuIndex = 0;
        }
      } else {
        inSubmenu = false;
        submenuIndex = 0;
      }
    }
    drawMenu();
    delay(150);
  }
}






void drawMenu() {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);

  if (!inSubmenu) {
    OLED.println("MENIU PRINCIPAL:");
    for (int i = 0; i < mainMenuCount; i++) {
      if (i == menuIndex) OLED.print("> ");
      else OLED.print("  ");
      OLED.println(mainMenu[i]);
    }
  } 
  else {
    switch (menuIndex) {
      case 0: // Setari
        OLED.println("Setari:");
        for (int i = 0; i < settingsCount; i++) {
          if (i < settingsCount - 1) {
            if (i == submenuIndex) OLED.print("> ");
            else OLED.print("  ");
            OLED.print(settingsMenu[i]);
            OLED.print(": ");
            OLED.println(settingsStates[i] ? "ON" : "OFF");
          } else {
            // "Inapoi"
            if (i == submenuIndex) OLED.print("> ");
            else OLED.print("  ");
            OLED.println(settingsMenu[i]);
          }
        }
        break;

      case 1: // Masuratori
        OLED.print("Masuratori: ");
        OLED.println(measurementsMenu[submenuIndex]);

        if (submenuIndex == 0) { // mmenu1
          for (int i = 0; i < sizeof(mmenu1Data) / sizeof(mmenu1Data[0]); i++) {
            OLED.println(mmenu1Data[i]);
          }
        } else if (submenuIndex == 1) { // mmenu2
          for (int i = 0; i < sizeof(mmenu2Data) / sizeof(mmenu2Data[0]); i++) {
            OLED.println(mmenu2Data[i]);
          }
        } else if (submenuIndex == 2) { // mmenu3 placeholder
          OLED.println("Date indisponibile...");
        } else {
          OLED.println("> Înapoi");
        }
        break;
      case 2:
        OLED.println("Conexiune:");
        OLED.println("SSID: MyWiFi");
        OLED.println("Stare: Conectat");
        OLED.println("> Înapoi");
        break;

      case 3:
        OLED.println("Info. hardware:");
        OLED.println("ESP32-WROOM32");
        OLED.println("RAM libera: 200KB");
        OLED.println("> Înapoi");
        break;

      case 4:
        OLED.println("Altele:");
        OLED.println("Versiune: v1.0");
        OLED.println("Build: 2025");
        OLED.println("> Înapoi");
        break;
    }
  }

  OLED.display();
}
