#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>

// CE and CSN pins
#define CE_PIN 4
#define CSN_PIN 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

int cindex=0;
int MainM_mode=0; // variabila utilizata pt. a bloca afisarea meniului de startup dupa ce se intra in meniul principal

const char* MainM[]={
  "Masuratori",
  "Conexiune",
  "Setari sist.",
  "Info. HW",
  "Alte opt."
};

const char* AP[]={
  "Temperatura: ",
  "Presiune: ",
  "Umiditate: ",
  "Int. lumin.: ",
  "Zgomot: ",
  //"Socuri: " //mecanice - nu exista spatiu aditional
};

const char* GM[]={
   "CO: ",
   "CO2: ",
   "Nox: ",
   "CH4: ",
   "LPG:"

}; // GAS MEASUREMENTS


const char* EM[] = {  // Electrical measurements
  "Tensiune (PS):",
  "Curent cons. : ",
  "Putere cons. : ",
  "Cap. Acc. : ",
  "Tensiune Acc. :"
};


const char* CONI[] = { // Connection info.
   "Stare: ",
   "Mod funct: ",
   "Viteza: ",
   "Putere: "
}

float ap[5]; // index 0 : - Temp, 1 - Presiunea, 2 - umiditate, 3 - int. lumin., 
float gm[5]; // gas measurements
float em[5]; // electrical measurements
char rcvdata[64];
//float temperature, pressure, humidity;

int MMlen=sizeof(MainM)/sizeof(MainM[0]);

void setup() {
  
  Serial.begin(115200);
  Serial.println("");
  pinMode(36, INPUT);
  pinMode(39, INPUT);
  pinMode(35, INPUT);
  pinMode(33, INPUT);
  pinMode(27, INPUT);
  pinMode(16, OUTPUT);
  // Serial.print(digitalRead(36));
  // Serial.print(digitalRead(39));
  // Serial.print(digitalRead(35));
  // Serial.print(digitalRead(33));
  // Serial.print(digitalRead(27));
  // Serial.println("");

  radio.begin();

  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  OLED.clearDisplay();

  for(int i=1;i<127;i++){
        OLED.drawPixel(i,1,WHITE);
    }

    for(int i=63;i>0;i--){
        OLED.drawPixel(127,i,WHITE);
    }

    for(int i=127;i>0;i--){
        OLED.drawPixel(i,63,WHITE);
    }

    for(int i=63;i>0;i--){
        OLED.drawPixel(1,i,WHITE);
    }
    
    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
    OLED.setCursor(5,10);     
    OLED.println("Terminal de receptie");
    OLED.setCursor(41,24);  
    OLED.println("SMCA-PA");
    OLED.println("");
    OLED.display();


    for(int i=34;i<94;i+=12){
        OLED.setCursor(i,50);
        OLED.print("*");
        delay(500);
        OLED.display();
    }
    delay(1200);
    OLED.clearDisplay();
    OLED.display(); 

    for(int i=0;i<sizeof(ap)/sizeof(ap[0]);i++){
      ap[i]=0.0;
      gm[i]=0.0;
    }
}
  



void data_rx(){
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();
  //char rcvdata[64] = {0};
  if (radio.available()) {
    radio.read(&rcvdata, sizeof(rcvdata));
    Serial.print("Received: ");
    Serial.println(rcvdata);
  }
  else{
    data_rx();
  }
}


void data_tx(char *txdata){
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);  // More reliable for longer distance
  radio.openWritingPipe(address);
  radio.stopListening();
  bool sentack = radio.write(&txdata, sizeof(txdata));
  
  if (sentack) {
    Serial.println("Sent successfully");
  } else {
    Serial.println("Sending failed");
  }
}



void displayStartup(){

    OLED.setCursor(49,2);
    OLED.print("MENIU");
    OLED.setCursor(37,12);
    OLED.println("PRINCIPAL");

    OLED.setCursor(47,30);
    OLED.println("ENTER");

    if(cindex==0){
      OLED.setCursor(35,30);
      OLED.println(">");
    }
    else{
      OLED.setCursor(35,30);
      OLED.println(" ");
    }

    OLED.setCursor(44,47);
    OLED.println("OPRIRE");

    if(cindex==1){
      OLED.setCursor(30,47);
      OLED.println(">");
    }
    else{
      OLED.setCursor(30,47);
      OLED.println(" ");
    }

    OLED.display();
    cursorInSM(2);

    if(!(digitalRead(35)) && cindex==0){
        OLED.clearDisplay();
        OLED.display();
        displayMainM();
    }

    else if(!(digitalRead(35)) && cindex==1){
        OLED.clearDisplay();
        OLED.display();
        // OPRIRE DISPLAY OLED
        delay(250);
        while(digitalRead(35)){} // bucla
    }

     displayStartup();
}

bool btnlast1=0;
bool btnlast2=0;

void cursorInSM(int n){
    n--;
    if(!digitalRead(36) && btnlast1==0){
      if(cindex==0){
        cindex=n;
      }
      else{
        cindex--;
      }
      OLED.clearDisplay();
      btnlast1=1;
      delay(100);
    }

    if(digitalRead(36)){
      btnlast1=0;
    }


    if(!digitalRead(27) && btnlast2==0){
      if(cindex==n){
        cindex=0;
      }
      else{
        cindex++;
      }
      OLED.clearDisplay();
      btnlast2=1;
      delay(100);
    }

    if(digitalRead(27)){
      btnlast2=0;
    }

    delay(5);  
}


void displayMainM(){
    
    for(int i=0,j=5;i<MMlen;i++,j+=12){
      if(cindex==i){
        OLED.setCursor(22,j);
        OLED.print(">");
      }
      else{
        OLED.setCursor(22,j);
        OLED.print(" ");
      }
      OLED.setCursor(32,j);
      OLED.print(MainM[i]);
      OLED.display();
    }
      
    if(!digitalRead(35)){
        switch(cindex){
            case 0:
              OLED.clearDisplay();
              OLED.display();
              cindex=0; // resetam variabila - reinitializam.
              delay(10);
              displayMeasurements();
              break;

            case 1:
              int xddc=0; //test
              break;
        }
    }

    cursorInSM(5);
    displayMainM();
}


void displayMeasurements(){
    
    OLED.setCursor(46,1);
    OLED.print("Pagini");

    OLED.setCursor(28,17);
    OLED.print("Masuratori 1");
    OLED.setCursor(20,17);
    if(cindex==0){
      OLED.print(">");
    }
    else{
    OLED.print(" ");
    }

    OLED.setCursor(28,29);
    OLED.print("Masuratori 2");
    OLED.setCursor(20,29);
    if(cindex==1){
      OLED.print(">");
    }
    else{
    OLED.print(" ");
    }

    OLED.setCursor(28,41);
    OLED.print("Masuratori 3");
    OLED.setCursor(20,41);
    if(cindex==2){
      OLED.print(">");
    }
    else{
    OLED.print(" ");
    }

    OLED.setCursor(32,53);
    OLED.print("Inapoi");
    OLED.setCursor(20,53);
    if(cindex==3){
      OLED.print(">");
    }
    else{
    OLED.print(" ");
    }

    delay(80);
    if(!digitalRead(35)){
        switch(cindex){
          case 0:
          // accesam pagina Masuratori 1 - gaze
            OLED.clearDisplay();
            OLED.display();
            displayGM();
            break;

          case 1:
            OLED.clearDisplay();
            OLED.display();
            displayAP();
            break;
         // accesam pagina Masuratori 2 - param. ambientali.

          case 2:
            OLED.clearDisplay();
            OLED.display();
            displayEM();
            break;

          case 3:
            OLED.clearDisplay();
            OLED.display();
            displayMainM();
            break;

          default:
            break;
        }
    }

    OLED.display();
    cursorInSM(4);
    displayMeasurements();
}



void displayAP(){

  data_tx("masuratori_2"); // cerem transmiterea de catre statie a masuratorilor 2
  delay(100);
  data_rx();

  // string rcvdata - cu formatul : "[20,22,2,13] - masuratorile 2 (ambientale)"

  for(int i=0,j=2;i<5;i++,j+=12){
    OLED.setCursor(7,j);
    OLED.print(AP[i]);
    OLED.setCursor(90,j);
    OLED.print(String(ap[i]));
  }

  if(!digitalRead(35)){
    delay(150);
    OLED.clearDisplay();
    OLED.display();
    displayMeasurements();
  }

  OLED.display();
  displayAP();
}



void displayGM(){
  data_tx("masuratori_1"); // cerem transmiterea de catre statie a masuratorilor 1
  delay(100);
  data_rx();

  // string rcvdata - cu formatul : "[20,22,2,13] - masuratorile 1"

  for(int i=0,j=2;i<5;i++,j+=12){
    OLED.setCursor(30,j);
    OLED.print(GM[i]);
    OLED.setCursor(60,j);
    OLED.print(String(gm[i]));
  }

  if(!digitalRead(35)){
    delay(150);
    OLED.clearDisplay();
    OLED.display();
    displayMeasurements();
  }

  OLED.display();
  displayGM();

}



void displayEM(){
  data_tx("masuratori_3"); // cerem transmiterea de catre statie a masuratorilor 3
  delay(100);
  data_rx();

  // string rcvdata - cu formatul : "[20,22,2,13] - masuratorile 3"

  for(int i=0,j=2;i<5;i++,j+=12){
    OLED.setCursor(7,j);
    OLED.print(EM[i]);
    OLED.setCursor(100,j);
    OLED.print(String(em[i]));
  }

  if(!digitalRead(35)){
    delay(150);
    OLED.clearDisplay();
    OLED.display();
    displayMeasurements();
  }

  OLED.display();
  displayEM();

}


void loop(){
  
  /*
    if(MainM_mode==0){
      displayStartup();
      cursorInSM(2);
      if(!(digitalRead(35)) && cindex==0){
        MainM_mode=1;
        OLED.clearDisplay();
        OLED.display();
        displayMainM();
      }
    } 

    displayStartup();

    else{
      displayMainM();
      cursorInSM(5);
      
      if(!digitalRead(35)){
        switch(cindex){
            case 0:
              OLED.clearDisplay();
              OLED.display();
              cindex=0; // resetam variabila - reinitializam.
              displayMeasurements();
              break;

            case 1:
              int xddc=0; //test
              break;
        }
      }
    } */

    displayStartup();

}
