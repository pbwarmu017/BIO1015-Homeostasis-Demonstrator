/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/

#include "headers.h"

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
int selectTimer = 0;

enum GAMESTATUS gameStatus = notstarted;
enum SYSTEMMODE systemMode = running;


//object and object pointer creation

_indicatorstrip Indicatorstrip; //object for the indicatorstrip
_handgrip Handgrip; //object for the handgrip
_encoder Handcrank; //object for the encoder
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield(); //object for the LCD

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

//LCD MENU SETUP
const int numOfScreens = 4;
int currentScreen = -1;
String screens[numOfScreens][2] = {{"OPTION 1","SELECTION 1"},
  {"OPTION 2","SELECTION 2"},{"OPTION 3","SELECTION 3"},{"OPTION 4","SELECTION 4"}};
int parameters[numOfScreens];


void navigateMenu(uint8_t button){
  // lcd.clear();
  // lcd.setCursor(0,1);
  // lcd.print(button);
  if(button & BUTTON_DOWN) {
    if(currentScreen == -1){
      currentScreen = 0;
    }
    else if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
      printScreen();
    }
    else{
      currentScreen--;
      printScreen();
    }
  }
  if(button & BUTTON_UP){
    if(currentScreen == -1){
      currentScreen = 0;
    }
    else if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
      printScreen();
    }
    else{
      currentScreen++;
      printScreen();
    }
  }
  if(button & BUTTON_LEFT && currentScreen != 0) {
    // parameterChange(0);
    printScreen();
  }
  if(button & BUTTON_RIGHT && currentScreen != 0) {
    // parameterChange(1);
    printScreen();
  }
}

void printScreen() {
  lcd.clear();
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0,1);
  // lcd.print(parameters[currentScreen]);
  // lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
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
  Indicatorstrip.initialize();
  Handcrank.initialize();

  //init serial for debugging  
  Serial.begin(2000000); 

  //init LCD
  lcd.begin(16, 2);

  //make unused pins float high to interrupting on stray voltages
  // for(int i = 0; i<)
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  // pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  // pinMode(5, INPUT_PULLUP);
  // pinMode(6, INPUT_PULLUP); //this pin is used for the LED strip
  pinMode(7, INPUT_PULLUP);
  pinMode(A0, INPUT);
}

void loop() {
  if(STRIPREFRESHDELAYFLAG){
    if(HANDGRIPACTIVE == 1){
      if(Handgrip.calibrationState == true){
        Indicatorstrip.setProductionRate(Handgrip.calculateProductionRate(
          analogRead(HANDGRIPPIN)), HANDGRIPDEVNUM);
        //set the indicator positions based on the production rate 
        Indicatorstrip.setIndicatorPosition(
          Handgrip.productionRate = Indicatorstrip.calculatePosition(HANDGRIPDEVNUM), HANDGRIPDEVNUM);
      } else {
        Handgrip.handgripMaxVoltage = Handgrip.voltageValue();
        Handgrip.calibrationState = true;
      }
    }
    //set the bounding box. 
    if(CRANKACTIVE == 1){
      Indicatorstrip.setIndicatorPosition(
        Handcrank.productionRate = Indicatorstrip.calculatePosition(CRANKDEVNUM), CRANKDEVNUM);
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
    uint8_t button = lcd.readButtons();
    if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == running){
      selectTimer++;
    }
    if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == running){
      systemMode = config;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("     CONFIG");
      lcd.setCursor(0,1);
      lcd.print("      MENU     ");
      lcd.setBacklight(RED);
      selectTimer = 0;
    }
    if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == config){
      selectTimer++;
    }
    if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == config){
      systemMode = running;
      lcd.setBacklight(WHITE);
      // currentScreen = -1;
      selectTimer = 0;
    }
    if(systemMode == running){
      lcd.setCursor(0,0);
      lcd.print("DCON1:");
      lcd.print(Handcrank.productionRate*21);
      lcd.setCursor(0,1);
      lcd.print("ACON1:");
      lcd.print(Handgrip.productionRate);
    }
    if(systemMode == config){
      navigateMenu(button);
    }
    // navigateMenu(button);
  LCDREFRESHFLAG = false;
  }
}
