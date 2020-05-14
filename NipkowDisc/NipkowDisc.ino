#include <avr/io.h>
#include <avr/interrupt.h>
#include "Arduino.h"
#include "Falling.h"


// defines for setting and clearing register bits



// each frame contains 32x64 pixels ( 2048)
// each pixel is represented as one byte in a file on the SDcard. Only the lower 6 bits are used.
// running at 16 frames/s about 32K bytes/s has to be read from the SDcard.
// the SD card can easily do that on average, but some reads take much long than average.
// To overcome this problem two 256 byte buffers are used. When the device is streaming bytes from one buffer the other is being filled from SD card.

#define SyncPixel              150
#define PinPulse               8  //  IR receiver circuit. (must be connected to  input capture pin 8 )

const byte NumberOfPixelBits   = 8;
volatile unsigned int Pixel = 0;    // current pixel index
volatile unsigned int lastT1capture, Period_Ticks; // used to calculate spinning frequency
volatile unsigned long NextOutputCompare32bit = 0;
volatile unsigned long PixelTime_16bitTickFraction = 1000000000;

Falling game = Falling();



void setup()
{
  
  pinMode(PinPulse, INPUT); 
  pinMode(A0, INPUT);// Configure input capture pin on timer1
  digitalWrite(PinPulse,0);
  pinMode(2, INPUT);//writing the LED
  pinMode(3, INPUT);//button1
  pinMode(4, OUTPUT);//button2
  pinMode(11, OUTPUT);//writing the IR emitter
  digitalWrite(11, LOW);// floating may have 50 Hz noise on it.
  cli();                    //clear interrupts while configuring timer1
  TCCR1A = 0;               //TC1 Control Register A 
  TCCR1B = B01000011;       //TC1 Control Register B : free running 250kHz counter, noise canceller enabled, falling edge input capture
  TIMSK1 = B00100010; 
  TCCR2A = _BV (COM2A0) | _BV(WGM21);
  TCCR2B = _BV (CS20);
  OCR2A =  209;
  ////~209 = ~38kHz | ~219 = ~36kHz//Timer/Counter 1 Interrupt Mask Register: Enable input capture Interupt (ICIE) , Enable Output Compare A Match Interrupt Enable (OCIEA)
  sei();        
  // enable interrupts
}

// timer 1 capture interrupt: function is triggered each time the synchronisation hole passes the light sensor.
ISR (TIMER1_CAPT_vect)
{ 

  unsigned int T1capture = ICR1 ;                    // read timer1 value when the sync occured
  Period_Ticks =  T1capture - lastT1capture; 
  // period = time between new and previous sync
  
  if (Period_Ticks > 5000 && Period_Ticks < 11000)                    // ignore very short periods
  {
    sei();   
    game.update();   
    // Increase max pixel clk by enabling nested interrupts (allows COMPA interrupt during sync interrupt)
    lastT1capture = T1capture ;
    signed int SyncError = Pixel - SyncPixel;      // We expect a certain pixel number when sync occurs. Calculate error.
    cli();                              // atomic update of outputcompareStep32bit
                                        // Calculate time between pixels = (time between syncs)/NumberOfPixels + correct part of the error. 
                                        // Expressed as 32bit value : 16 bit integer, 16bit fraction -> OutputCompareStep32bit
    PixelTime_16bitTickFraction  = ( ( long)(Period_Ticks) << (16 - NumberOfPixelBits) ) + ((signed long)(SyncError)<< 9); // each sync a fraction of the error is corrected : 
  } 

}


ISR(TIMER1_COMPA_vect)
{ 
  PORTD = ((game.disp.Pixel[Pixel/16][Pixel%16] && 0x3F)<<4);
  
  OCR1A = (NextOutputCompare32bit >> 16);                          // set new output compare value ( = integer part of OutputCompareStep32bit )
  NextOutputCompare32bit += (PixelTime_16bitTickFraction);         // calculate new output compare value
                       // trigger to load low bank from SDcard to buffer ( currently reading from high bank )
  Pixel++;                                                         // next pixel
  if (Pixel > ((1 << NumberOfPixelBits) - 1)) Pixel = 0;
}


void loop() { 
  
  
}      
