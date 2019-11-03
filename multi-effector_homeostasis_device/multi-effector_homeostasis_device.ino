/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/

//main pin defines
#include <Arduino.h>
//neopixels
#include <Adafruit_NeoPixel.h>
//RGB LCD Display
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
//custom libaries
#include "multi-effector_homeostasis_device.h"
#include "ledstrip.h"
#include "handgrip.h"
#include "encoder.h"

//used to track timer overflows for refreshing the strip
volatile unsigned int stripDelayCounter = 0; 
volatile int prevOut = 0;
volatile int crankRateCalcDelayCounter = 50;
volatile int gameResetCounter = 0;
volatile int lcdRefreshCounter = 0;
volatile int crankSum = 0;
volatile bool STRIPREFRESHDELAYFLAG = false;
volatile bool CRANKRATECALCDELAYFLAG = false;
volatile bool LCDREFRESHFLAG = false;
volatile bool RESETFLAG = false;

enum GAMESTATUS gameStatus = notstarted;
enum SYSTEMMODE systemMode = NONE;

_indicatorstrip Indicatorstrip; //object for the indicatorstrip
_handgrip Handgrip; //object for the handgrip
_encoder Handcrank; //object for the encoder
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield(); //opbject for the LCD

//this is the interrupt handler for Timer0 output conpare match. 
ISR(TIMER0_COMPA_vect) { //this executes every 1 millisecond
  stripDelayCounter++;
  crankRateCalcDelayCounter++;
  lcdRefreshCounter++;
  if(stripDelayCounter >= STRIPREFRESHDELAY) {  
    stripDelayCounter = 0; //reset the timer counter for the next run.
    STRIPREFRESHDELAYFLAG = true;
    //Set Rates based on affector positions (one for each affector)
  }
  if(crankRateCalcDelayCounter >= CRANKRATECALCDELAY){
    crankRateCalcDelayCounter = 0; 
    CRANKRATECALCDELAYFLAG = true;
    
  }
  if(gameStatus == lost){
    gameResetCounter++;
    if(gameResetCounter >= GAMERESETDELAY){
      gameResetCounter = 0;
      gameStatus = notstarted;
      RESETFLAG = true;

    }
  }

  if(lcdRefreshCounter >= LCDREFRESHDELAY){
    LCDREFRESHFLAG = true;  
    lcdRefreshCounter = 0;  
  }
}

ISR(PCINT2_vect) { // handle pin change interrupt for D0 to D7 here
  if(CRANKACTIVE == 1){
    int currentOut = Handcrank.returnDelta();
      //make sure it's not an invalid state change
      if(currentOut){ 
        //two or more matching values. Helps with logical debounce
        if(currentOut == prevOut){ 
          crankSum += currentOut;
        }
        prevOut = currentOut; //update the previous value
    }
  }
}

void navigateMenu(uint8_t button){
  if (button) {
    lcd.clear();
    lcd.setCursor(0,1);
    // lcd.print(button);
    if (button & BUTTON_UP) {
      lcd.print("UP ");
      lcd.setBacklight(RED);
    }
    if (button & BUTTON_DOWN) {
      lcd.print("DOWN ");
      lcd.setBacklight(YELLOW);
    }
    if (button & BUTTON_LEFT) {
      lcd.print("LEFT ");
      lcd.setBacklight(GREEN);
    }
    if (button & BUTTON_RIGHT) {
      lcd.print("RIGHT ");
      lcd.setBacklight(TEAL);
    }
    if (button & BUTTON_SELECT) {
      lcd.print("SELECT ");
      lcd.setBacklight(VIOLET);
    }
  }
}

void setup() {
  //setup timer0 to call interrupt OCR0A every REFRESHTIMEVAL
  OCR0A = 0xFA; //set to trigger TIMER0_COMPA_vect every millisecond
  TIMSK0 |= _BV(OCIE0A); //enable the output compare interrupt on timer0

  //setup interrupt on pin change for the ender pins.
  //This interrupt is enabled per group (digital pins 1-7, digtial pins 8-13)
  // enable pin change interrupt for encoder pin A
  *digitalPinToPCMSK(ENCODERPINA) |= bit (digitalPinToPCMSKbit(ENCODERPINA)); 
  // enable ping change interrupt for encoder pin B
  *digitalPinToPCMSK(ENCODERPINB) |= bit (digitalPinToPCMSKbit(ENCODERPINB)); 
  // clear any outstanding interrupt flag

  PCIFR  |= bit (digitalPinToPCICRbit(ENCODERPINA)); 
  // enable interrupt for the GROUP 
  PCICR  |= bit (digitalPinToPCICRbit(ENCODERPINA)); 
  systemMode = NONE;
  Indicatorstrip.initialize();
  Handcrank.initialize();

  //init serial for debugging  
  Serial.begin(2000000); 

  //init LCD
  lcd.begin(16, 2);

  //make unused pins float high to interrupting on stray voltages
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  // pinMode(6, INPUT_PULLUP); //this pin is used for the LED strip
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  if(STRIPREFRESHDELAYFLAG){
    if(HANDGRIPACTIVE == 1){
      if(Handgrip.calibrationState == true){
        Indicatorstrip.setProductionRate(Handgrip.calculateProductionRate(
          analogRead(HANDGRIPPIN)), HANDGRIPDEVNUM);
        //set the indicator positions based on the production rate 
        Indicatorstrip.setIndicatorPosition(
          Indicatorstrip.calculatePosition(HANDGRIPDEVNUM), HANDGRIPDEVNUM);
      } else {
        Handgrip.handgripMaxVoltage = Handgrip.voltageValue();
        Handgrip.calibrationState = true;
      }
    }
    //set the bounding box. 
    if(CRANKACTIVE == 1){
      Indicatorstrip.setIndicatorPosition(
        Indicatorstrip.calculatePosition(CRANKDEVNUM), CRANKDEVNUM);
    }
    Indicatorstrip.setBoundingBox(BOXSTART, BOXSIZE);
    Indicatorstrip.update();
    STRIPREFRESHDELAYFLAG = false;
  }
  if(CRANKRATECALCDELAYFLAG){
    if(CRANKACTIVE == 1){
      Indicatorstrip.setProductionRate(Handcrank.calculateProductionRate(
        crankSum), CRANKDEVNUM);
      //reset the sum because it has just been incorporated into a moving avg
      crankSum = 0; 
      //reset the delay counter for the next run
      CRANKRATECALCDELAYFLAG = false;
    }
  }
  if(RESETFLAG){
    Indicatorstrip.setBoundingBox(BOXSTART, BOXSIZE);
    RESETFLAG = false;
  }
  if(LCDREFRESHFLAG){
    // (note: line 1 is the second row, since counting begins with 0)
    // uint8_t button = lcd.readButtons();
    // navigateMenu(button);
    // lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Crank Rate: ");
    lcd.print(Handcrank.productionRate);
    lcd.setCursor(0,1);
    lcd.print("Sqze. Rate: ");
    lcd.print(Handgrip.productionRate);
    LCDREFRESHFLAG = false;
  }
}
