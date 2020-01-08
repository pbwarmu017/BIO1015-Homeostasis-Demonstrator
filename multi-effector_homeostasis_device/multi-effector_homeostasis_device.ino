/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/
#include "superclasses.cpp"
#include "encoder.cpp"
#include "handgrip.cpp"
#include "lcd.cpp"
#include "indicatorstrip.cpp"
#include "menu.cpp"

enum GAMESTATUS gameStatus = notstarted;
enum SYSTEMMODE systemMode = running;

// #include "parents.cpp"
// #include "encoder.cpp"
// #include "handgrip.cpp"
// #include "lcd.cpp"
// #include "ledstrip.cpp"
// #include "menu.cpp"

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

int selectTimer = 0;
bool INITIALSETUPFLAG;

//generic pointer declarations
_device *DCON1_ptr;
_device *ACON1_ptr;
_device *DACON1_ptr;
_device *DCON2_ptr;
_device *ACON2_ptr;
_device *DACON2_ptr;

//generic pointer used in createObject()
_device *gen_ptr;

_device *Indicatorstrip_ptr = new _indicatorstrip; //object for the indicatorstrip
_device *menu_ptr = new _menu; //object for the menu system
_device *lcd_ptr;

_device *Handgrip_ptr = new _handgrip; //object for the handgrip
_device *Handcrank_ptr = new _encoder; //object for the encoder
//this is the interrupt handler for Timer0 output conpare match. 

_device* createObject(int objtype, int portnum){
  if(objtype == lcd_type){
    gen_ptr = new _lcd;
    return(gen_ptr);
  }
}
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
    int currentOut = ((_encoder  *)Handcrank_ptr)->returnDelta();
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

void setup() {

  // //init serial for debugging  
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
  // Indicatorstrip_ptr->initialize();
  // Handcrank_ptr->initialize();

  //make all digital pins float high to prevent interrupting on stray voltages
 
  // pinMode(0, INPUT_PULLUP);
  // pinMode(1, INPUT_PULLUP);
  // pinMode(2, INPUT_PULLUP);
  // pinMode(3, INPUT_PULLUP);
  // pinMode(4, INPUT_PULLUP);
  // pinMode(5, INPUT_PULLUP);
  // pinMode(6, INPUT_PULLUP);
  // pinMode(7, INPUT_PULLUP);
  // pinMode(A0, INPUT);

  lcd_ptr = createObject(lcd_type, HARDCODED_PORTNUM);
  // Serial.print(lcd_ptr);
  (((_lcd *)lcd_ptr)->lcd_obj)->begin(16, 2);
  (((_lcd *)lcd_ptr)->lcd_obj)->setBacklight(WHITE);
}

void loop() {
  // if(STRIPREFRESHFLAG){
  //   if(HANDGRIPACTIVE == 1){
  //     if->Handgrip_ptrcalibrationState == true){
  //      ->Indicatorstrip_ptrsetProductionRate->Handgrip_ptrcalculateProductionRate(
  //         analogRead(HANDGRIPPIN), Handgrip_ptr), HANDGRIPDEVNUM);
  //       //set the indicator positions based on the production rate 
  //      ->Indicatorstrip_ptrsetIndicatorPosition(
  //        ->Handgrip_ptrproductionRate =->Indicatorstrip_ptrcalculatePosition(HANDGRIPDEVNUM), HANDGRIPDEVNUM);
  //     } else {
  //      ->Handgrip_ptrhandgripMaxVoltage =->Handgrip_ptrvoltageValue();
  //      ->Handgrip_ptrcalibrationState = true;
  //     }
  //   }
  //   //set the bounding box. 
  //   if(CRANKACTIVE == 1){
  //    ->Indicatorstrip_ptrsetIndicatorPosition(
  //      ->Handcrank_ptrproductionRate =->Indicatorstrip_ptrcalculatePosition(CRANKDEVNUM), CRANKDEVNUM);
  //   }
  //  ->Indicatorstrip_ptrsetBoundingBox(BOXSTART, BOXSIZE);
  //  ->Indicatorstrip_ptrupdate();
  //   STRIPREFRESHFLAG = false;
  // }
  // if(CRANKRATECALCFLAG){
  //   if(CRANKACTIVE == 1){
  //    ->Indicatorstrip_ptrsetProductionRate->Handcrank_ptrcalculateProductionRate(
  //       crankSum), CRANKDEVNUM);
  //     //reset the sum because it has just been incorporated into a moving avg
  //     crankSum = 0; 
  //     //reset the delay counter for the next run
  //     CRANKRATECALCFLAG = false;
  //   }
  // }
  // if(RESETFLAG){
  //  ->Indicatorstrip_ptrsetBoundingBox(BOXSTART, BOXSIZE);
  //   RESETFLAG = false;
  // }
  if(LCDREFRESHFLAG){
    // (note: line 1 is the second row, since counting begins with 0)
    uint8_t button = (((_lcd *)lcd_ptr)->lcd_obj)->readButtons();
    if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == running){
      selectTimer++;
    }
    if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == running){
      systemMode = config;
      Serial.print("ENTERING MENU");
      (((_lcd *)lcd_ptr)->lcd_obj)->clear();
      ((_menu *)menu_ptr)->currentScreen = 0;
      ((_menu *)menu_ptr)->printMenu( (_lcd*)lcd_ptr );

      (((_lcd *)lcd_ptr)->lcd_obj)->setBacklight(RED);
      selectTimer = 0;
    }
    if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == config){
      selectTimer++;
    }
    if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == config){
      systemMode = running;
      (((_lcd *)lcd_ptr)->lcd_obj)->setBacklight(WHITE);
      selectTimer = 0;
    }
    if(systemMode == running){
      (((_lcd *)lcd_ptr)->lcd_obj)->clear();
      // (((_lcd *)lcd_ptr)->lcd_obj)->setCursor(0,0);
      // (((_lcd *)lcd_ptr)->lcd_obj)->print("System");
      // (((_lcd *)lcd_ptr)->lcd_obj)->setCursor(0,1);
      // (((_lcd *)lcd_ptr)->lcd_obj)->print("Operational");
    }
    if(systemMode == config){
      ((_menu *)menu_ptr)->navigateMenu(button, (_lcd*)lcd_ptr);
    }
    // navigateMenu(button);
  LCDREFRESHFLAG = false;
  }
}
