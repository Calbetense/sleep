#include <Arduino.h>

#define LED       5
#define sleep     __asm__ __volatile__ ("sleep" "\n\t" :: );
#define wdt_reset __asm__ __volatile__ ("wdr" "\n\t" :: );

void blk_wdt();

void setup() {
  //Set up led
  DDRB |= (1 << LED);
  
  //Set up sleep mode (start with power down)
  SMCR |= (1 << SM1); 
  //Turn off innecesaries modules 
  PRR |= (1 << PRTWI); // Disconnect I2C when sleep
  //Set up watch dog timer
  cli();
  wdt_reset;

  WDTCSR = (1 << WDCE) | (1 << WDE);      // Allows configure wdt in 4 clk period
  WDTCSR = (1 << WDIE) | (11 << WDP1);    // Interrupt mode, 1 second period

  sei();
  
  //Sleep
  SMCR |= (1 << SE); //Sleep
  sleep;
  
/***************************/
  while(1){
    //Blink routine with wdt
    blk_wdt();
  }
/***************************/ 
}

void blk_wdt(){
  //Toggle led
  PORTB ^= (1 << LED);
  //wdt reset
  wdt_reset;
  //Sleep
  SMCR |= (1 << SE);
  sleep;
}

ISR(WDT_vect){
  SMCR &= ~(1 << SE); // Disable Sleep Mode  
}