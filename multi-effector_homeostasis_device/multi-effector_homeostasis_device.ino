/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/

#include "headers.h"

//used to track timer overflows for refreshing the strip
volatile unsigned stripDelayCounter = 0; 
volatile int prevOut = 0;
volatile int crankRateCalcDelayCounter = 50;
volatile int gameResetCounter = 0;
volatile int lcdRefreshCounter = 0;
volatile int crankSum = 0;
volatile bool STRIPREFRESHFLAG = false;
volatile bool CRANKRATECALCFLAG = false;
volatile bool LCDREFRESHFLAG = false;
volatile bool RESETFLAG = false;

//this is a global placeholders for pointers. 

unsigned int *temp_ptr = 0;
// unsigned int DCON1_ptr = 0;
// unsigned int ACON1_ptr = 0;
// unsigned int DACON1_ptr = 0;
// unsigned int DCON2_ptr = 0;
// unsigned int ACON2_ptr = 0;
// unsigned int DACON2_ptr = 0;
// unsigned int *lcd_ptr = 0;

int selectTimer = 0;

enum GAMESTATUS gameStatus = notstarted;
enum SYSTEMMODE systemMode = running;


_indicatorstrip *Indicatorstrip_ptr = new _indicatorstrip; //object for the indicatorstrip
_handgrip *Handgrip_ptr = new _handgrip; //object for the handgrip
_encoder *Handcrank_ptr = new _encoder; //object for the encoder
_menu *menu_ptr = new _menu; //object for the menu system

//this is the interrupt handler for Timer0 output conpare match. 
ISR(TIMER0_COMPA_vect) { //this executes every 1 millisecond
  stripDelayCounter++;
  crankRateCalcDelayCounter++;
  lcdRefreshCounter++;
  if(stripDelayCounter >= STRIPREFRESHDELAY) {  
    stripDelayCounter = 0; //reset the timer counter for the next run.
    STRIPREFRESHFLAG = true;
    //Set Rates based on affector positions (one for each affector)
  }
  if(crankRateCalcDelayCounter >= CRANKRATECALCDELAY){
    crankRateCalcDelayCounter = 0; 
    CRANKRATECALCFLAG = true;
    
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
    int currentOut = (*Handcrank_ptr).returnDelta();
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

unsigned int createObject(byte objtype, byte portnum) {
  if(objtype == lcd_type) {
    unsigned int ptr = new Adafruit_RGBLCDShield();
    return(ptr);
  }
  return(0);
}

void setup() {

  //init serial for debugging  
  Serial.begin(2000000); 

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
  (*Indicatorstrip_ptr).initialize();
  (*Handcrank_ptr).initialize();

  //make all digital pins float high to prevent interrupting on stray voltages
 
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  // pinMode(A0, INPUT);
  bool INITIALSETUPFLAG = true;
}

void loop() {
  if(INITIALSETUPFLAG){
    Adafruit_RGBLCDShield *lcd_ptr = createObject(lcd_type, HARDCODED_PORTNUM);
  //init LCD
    (*lcd_ptr).begin(16, 2);
  }
  // if(STRIPREFRESHFLAG){
  //   if(HANDGRIPACTIVE == 1){
  //     if((*Handgrip_ptr)calibrationState == true){
  //       (*Indicatorstrip_ptr)setProductionRate((*Handgrip_ptr)calculateProductionRate(
  //         analogRead(HANDGRIPPIN), Handgrip_ptr), HANDGRIPDEVNUM);
  //       //set the indicator positions based on the production rate 
  //       (*Indicatorstrip_ptr)setIndicatorPosition(
  //         (*Handgrip_ptr)productionRate = (*Indicatorstrip_ptr)calculatePosition(HANDGRIPDEVNUM), HANDGRIPDEVNUM);
  //     } else {
  //       (*Handgrip_ptr)handgripMaxVoltage = (*Handgrip_ptr)voltageValue();
  //       (*Handgrip_ptr)calibrationState = true;
  //     }
  //   }
  //   //set the bounding box. 
  //   if(CRANKACTIVE == 1){
  //     (*Indicatorstrip_ptr)setIndicatorPosition(
  //       (*Handcrank_ptr)productionRate = (*Indicatorstrip_ptr)calculatePosition(CRANKDEVNUM), CRANKDEVNUM);
  //   }
  //   (*Indicatorstrip_ptr)setBoundingBox(BOXSTART, BOXSIZE);
  //   (*Indicatorstrip_ptr)update();
  //   STRIPREFRESHFLAG = false;
  // }
  // if(CRANKRATECALCFLAG){
  //   if(CRANKACTIVE == 1){
  //     (*Indicatorstrip_ptr)setProductionRate((*Handcrank_ptr)calculateProductionRate(
  //       crankSum), CRANKDEVNUM);
  //     //reset the sum because it has just been incorporated into a moving avg
  //     crankSum = 0; 
  //     //reset the delay counter for the next run
  //     CRANKRATECALCFLAG = false;
  //   }
  // }
  // if(RESETFLAG){
  //   (*Indicatorstrip_ptr)setBoundingBox(BOXSTART, BOXSIZE);
  //   RESETFLAG = false;
  // }
  if(LCDREFRESHFLAG){
    // (note: line 1 is the second row, since counting begins with 0)
    uint8_t button = (*lcd_ptr).readButtons();
    if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == running){
      selectTimer++;
    }
    if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == running){
      systemMode = config;
      (*lcd_ptr).clear();
      (*menu_ptr).currentScreen = 0;
      (*menu_ptr).printMenu(lcd_ptr);

      (*lcd_ptr).setBacklight(RED);
      selectTimer = 0;
    }
    if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == config){
      selectTimer++;
    }
    if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == config){
      systemMode = running;
      (*lcd_ptr).setBacklight(WHITE);
      selectTimer = 0;
    }
    if(systemMode == running){
      (*lcd_ptr).clear();
      // lcd_ptr->setCursor(0,0);
      // lcd_ptr->print("System");
      // lcd_ptr->setCursor(0,1);
      // lcd_ptr->print("Operational");
    }
    if(systemMode == config){
      (*menu_ptr).navigateMenu(button, lcd_ptr);
    }
    // navigateMenu(button);
  LCDREFRESHFLAG = false;
  }
}
