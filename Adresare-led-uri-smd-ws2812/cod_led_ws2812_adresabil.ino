#define LED_PIN 6
#define LED_PORT PORTD
#define LED_DDR  DDRD
#define LED_MASK (1 << LED_PIN)

void ws2812_send_byte(uint8_t byte);
void ws2812_send_pixel(uint8_t r, uint8_t g, uint8_t b);
long g,r,b=0;


void setup() {
    LED_DDR |= LED_MASK;   // Set pin as output
    randomSeed(analogRead(0));
}

void loop() {

    int n = random(5);

    delay(100);
    
    if(n==1){
      int pg=g;
      delay(50);
      g = random(200);

      if(pg-30<g){
        for(int i=pg;i<g;i++){
          ws2812_send_pixel(i, r, b);
          delay(60);
        }
      }
      
      else if(pg<g){
        for(int i=pg;i<g;i++){
          ws2812_send_pixel(i, r, b);
          delay((g-pg)+1);
        }
      }

      else if(pg+30>g){
        for(int i=pg;i<g;i++){
          ws2812_send_pixel(i, r, b);
          delay(60);
        }
      }

      else if(pg>g){
        for(int i=pg;i>g;i--){
          ws2812_send_pixel(i, r, b);
          delay((pg-g)+1);
        }
      }
      
    }

    else if(n==2){
      int pr=r;
      delay(50);
      r = random(200);
     /*  if(pr-30<r){ //pt a avea timp de incrementare minim - eliminam trecerile bruste de la o valoare la alta pt valori aproapiate (anterioara, curenta).
        while(r>pr-30){
          r = random(200);
        }
      } */

      if(pr-30<r){
        for(int i=pr;i<r;i++){
          ws2812_send_pixel(g, i, b);
          delay(60);
        }
      }

      else if(pr<r){
        for(int i=pr;i<r;i++){
          ws2812_send_pixel(g, i, b);
          delay((r-pr)+1);
        }
      }

      else if(pr+30>r){
        for(int i=pr;i<r;i++){
          ws2812_send_pixel(g, i, b);
          delay(60);
        }
      }

      else if(pr>r){
        for(int i=pr;i>r;i--){
          ws2812_send_pixel(g, i, b);
          delay((pr-r)+1);
        }
      }
    }

    else if(n==3){
      int pb=b;
      delay(50);
      b = random(200);
      
      if(pb-30<b){
        for(int i=pb;i<b;i++){
          ws2812_send_pixel(g, r, i);
          delay(60);
        }
      }

      else if(pb<b){
        for(int i=pb;i<b;i++){
          ws2812_send_pixel(g, r, i);
          delay((b-pb)+1);
        }
      }

      else if(pb+30>b){
        for(int i=pb;i<b;i++){
          ws2812_send_pixel(g, r, i);
          delay(60);
        }
      }


      else if(pb>b){
        for(int i=pb;i>b;i--){
          ws2812_send_pixel(g, r, i);
          delay((pb-b)+1);
        }
      }
    }


   // ws2812_send_pixel(g, r, b); 
    

}

/* -------------------------------------------------- */

void ws2812_send_pixel(uint8_t r, uint8_t g, uint8_t b) {
    cli();          // Disable interrupts during data
    ws2812_send_byte(g);   // WS2812 uses GRB order
    ws2812_send_byte(r);
    ws2812_send_byte(b);
    sei();          // Re-enable interrupts
    delayMicroseconds(60); // Reset latch (>50 µs)
}

void ws2812_send_byte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        if (byte & 0x80) {
            // Send '1'
            asm volatile (
                "sbi %[port], %[pin] \n\t"
                "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
                "cbi %[port], %[pin] \n\t"
                "nop\n\t""nop\n\t""nop\n\t"
                :
                : [port] "I" (_SFR_IO_ADDR(LED_PORT)),
                  [pin] "I" (LED_PIN)
            );
        } else {
            // Send '0'
            asm volatile (
                "sbi %[port], %[pin] \n\t"
                "nop\n\t""nop\n\t"
                "cbi %[port], %[pin] \n\t"
                "nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t"
                :
                : [port] "I" (_SFR_IO_ADDR(LED_PORT)),
                  [pin] "I" (LED_PIN)
            );
        }
        byte <<= 1;
    }
}
