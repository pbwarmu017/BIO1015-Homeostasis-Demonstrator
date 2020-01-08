/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>


enum GAMESTATUS {
  notstarted,
  started,
  lost
};

enum SYSTEMMODE {
  calibrate,
  running,
  config
};

//port number defines for readability 
#define DCON1_PORTNUM 1
#define ACON1_PORTNUM 2
#define DACON1_PORTNUM 3
#define DCON2_PORTNUM 4
#define ACON2_PORTNUM 5
#define DACON2_PORTNUM 6
#define HARDCODED_PORTNUM 7 //this is used when the port is hardcoded in the library (lcd)

#define indicatorStrip_type 1
#define handGrip_type 2
#define handCrank_type 3
#define lcd_type 4
#define menu_type 5

extern enum GAMESTATUS gameStatus;
extern enum SYSTEMMODE systemMode;

#define HANDGRIPPIN A0
#define ENCODERPINA 3
#define ENCODERPINB 5

#define HANDGRIPDEVNUM 0 //device number for handgrip
#define CRANKDEVNUM 1

#define HANDGRIPACTIVE 1
#define CRANKACTIVE 1

#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

#define LCDREFRESHDELAY 205 //in milliseconds

#define SELECTTIMERROLLOVER 5

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//LEDSTRIP
#define LED_PIN 6
#define LED_COUNT 60 //the number of LEDs on the strip
#define BOXSTART 22 //bounding box default starting position
#define BOXSIZE 10 //bounding box default size

//LED COLOR CODE DEFINITIONS. 
#define COLORRED strip.Color(128,0,0) //USED FOR PRE GAME INDICATION
#define COLORORANGE strip.Color(128,64,0) //USED FOR SECONDINDICATORCOLOR
#define COLORYELLOW strip.Color(128,128,0)
#define COLORGREEN strip.Color(0,128,0) //USED FOR "GAME ON" INDICATION
#define COLORBLUE strip.Color(0,0,128) //USED FOR SQUEEZEINDICATORCOLOR
#define COLORPURPLE strip.Color(25,0,51)
#define COLORCYAN strip.Color(0,128,128)

//AFFECTOR INDICATOR COLOR SELECTIONS
#define SQUEEZEINDICATORCOLOR COLORBLUE
#define CRANKINDICATORCOLOR COLORORANGE
//DEFAULT RATES
#define DEFAULTINDICATORPOSITION 0//BOXSTART+round((float)BOXSIZE/2)
#define DEFAULTPRODUCTIONRATE 25 
#define DEFAULTCONSUMPTIONRATE 50
//REFRESH SPEEDS
#define STRIPREFRESHDELAY 50 //in milliseconds
#define LEDMAXINCREMENT 5 //Max # of LEDs to jump per cycle
//END LEDSTRIP

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

//this is a global placeholders for pointers. 

unsigned int *temp_ptr = 0;


int selectTimer = 0;

enum GAMESTATUS gameStatus = notstarted;
enum SYSTEMMODE systemMode = running;

bool INITIALSETUPFLAG;


// _device* createObject(byte objtype, byte portnum) {

//   return(0);
// }
class _device {
  public:
    // return(0);`
};

class _affector: public _device {
  public:
    float returnPosition(float currentPosition){
      return(currentPosition);
    }
};

class _menu: public _device{
  public:
    static const int numOfScreens = 4;  //number of options in screens[]
    static const int numOfModes = 2; //number of options in modescreens[]
    static const int numOfPorts = 6; //number of options in portscreens[]
    int currentScreen = 0;
    //The following are selection tracking variables. As the user uses the
    //menu the contents of these values will change.
    int sqafport = 0;
    int sqafmode = 0;
    int crafport = 0;
    int crafmode = 0;

    //LCD MENU TEXT OPTIONS

    //Main Menu Screens
    String screens[numOfScreens][2] = {{"SQUEEZE AFFECTOR","MODE: "},
      {"SQUEEZE AFFECTOR","PORT: "},{"CRANK AFFECTOR","MODE:"},{"CRANK AFFECTOR","PORT: "}};

    //Mode Selections. If you add to this, increase numOfModes to match.
    String modescreens[2] = {"OFF","ON"};

    //Port Selections. If you add to this, increase numOfPorts to match. 
    String portscreens[6] = {"DCON1","ACON1","DACON1","DCON2","ACON2","DACON2"};
    
    int navigateMenu(int button, Adafruit_RGBLCDShield* lcd){
      if(button & BUTTON_UP) {
        if (currentScreen == 0) {
              currentScreen = numOfScreens-1;
              printMenu(lcd);
            }
            else{
              currentScreen--;
              printMenu(lcd);
            }
      }
      if(button & BUTTON_DOWN){
        if (currentScreen == numOfScreens-1) {
              currentScreen = 0;
              printMenu(lcd);
            }
        else{
          currentScreen++;
          printMenu(lcd);
        }
      }
      if(button & BUTTON_LEFT) {
        parameterChange(0);
        printMenu(lcd);
      }
      if(button & BUTTON_RIGHT) {
        parameterChange(1);
        printMenu(lcd);
      }
      return(0);
    }
    void parameterChange(int index) {
      if(index == 1){
        if(currentScreen == 0){
          if(sqafmode < numOfModes-1) sqafmode++;
          else sqafmode = 0;
        }
        if(currentScreen == 1) {
          if(sqafport < numOfPorts-1) sqafport++;
          else sqafport = 0;
        }
        if(currentScreen == 2) {
          if(crafmode < numOfModes-1) crafmode++;
          else crafmode = 0;
        }
        if(currentScreen == 3) {
          if(crafport < numOfPorts-1) crafport++;
          else crafport = 0;
        }
      }
      if(index == 0){
        if(currentScreen == 0) {
          if(sqafmode > 0) sqafmode--;
          else sqafmode = numOfModes-1;
        }
        if(currentScreen == 1) {
          if(sqafport > 0) sqafport--;
          else sqafport = numOfPorts-1;
        } 
        if(currentScreen == 2) {
          if(crafmode > 0) crafmode--;
          else crafmode = numOfModes-1;
        }
        if(currentScreen == 3) {
          if(crafport > 0) crafport--;
          else crafport = numOfPorts-1;
        }
      }
    }
    void printMenu(Adafruit_RGBLCDShield *lcd) {
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print(screens[currentScreen][0]);
      lcd->setCursor(0,1);
      lcd->print(screens[currentScreen][1]);
      lcd->print(" ");
      if(currentScreen == 0){ //squeeze affector mode screen
        lcd->print(modescreens[sqafmode]);
      }
      if(currentScreen == 1){ //squeeze affector port screen
        if(sqafmode == 1){
          lcd->print(portscreens[sqafport]);
        }
        else{
          lcd->print("N/A");
        }
      }
      if(currentScreen == 2){ //crank affector mode screen
        lcd->print(modescreens[crafmode]);
      }
      if(currentScreen == 3){ //crank affector port screen
        if(crafmode == 1){
          lcd->print(portscreens[crafport]);
        }
        else{
          lcd->print("N/A");
        }
      }
    }
};

class _indicatorstrip: public _device {
  Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

  public:
     /*VARIABLES*/

    int boxSize = 3; //the size of the box centered around boxposition (must be at least 3)
    int boxStart = BOXSTART; //the position of the bounding box as an LED number the box is centered around. 
    int boxLowerBound;
    int boxUpperBound;
    float crankIndicatorPosition = DEFAULTINDICATORPOSITION;
    double prevSqueezeIndPos = -1;
    double prevCrankIndPos = -1;
    float squeezeIndicatorPosition = DEFAULTINDICATORPOSITION; 
    /*FUNCTIONS*/
    float calculatePosition(int devnum) {
      if(devnum == HANDGRIPDEVNUM){
        float delta = squeezeProductionRate-consumptionRate;
        return(squeezeIndicatorPosition+(LEDMAXINCREMENT * delta));  
      }
      if(devnum == CRANKDEVNUM){
        float delta = crankProductionRate-consumptionRate;
        return(crankIndicatorPosition+(LEDMAXINCREMENT * delta));
      }
      return 0;
    }
    bool indicatorsWithinBounds(void) {
      //checking for the squeeze indicator
      bool statusVariable = true;

      if(HANDGRIPACTIVE == 1){
        if(squeezeIndicatorPosition <= boxLowerBound){
          losingColor = SQUEEZEINDICATORCOLOR;
          statusVariable = false;
        }
        if(squeezeIndicatorPosition >= boxUpperBound) {
          losingColor = SQUEEZEINDICATORCOLOR;
          statusVariable = false;
        }
      }

      //checking for the hand crank indicator
      if(CRANKACTIVE == 1){
        if(crankIndicatorPosition <= boxLowerBound){
          losingColor = CRANKINDICATORCOLOR;
          statusVariable =  false;
        }

        if(crankIndicatorPosition >= boxUpperBound) {
          losingColor = CRANKINDICATORCOLOR;
          statusVariable =  false;
        }
      }
      return(statusVariable);
    }
    void initialize(void) {
      strip.begin();
      strip.show();
      return;
    }
    void setBoundingBox(int boxstart, int boxsize) {
      boxLowerBound = boxstart;
      boxUpperBound = boxstart+boxsize+1;


      if(gameStatus == started && indicatorsWithinBounds()){
        strip.setPixelColor(boxstart, COLORGREEN);
        strip.setPixelColor(boxstart+boxsize+1, COLORGREEN);
      }
      if(gameStatus == started && !indicatorsWithinBounds()){
        gameStatus = lost;
        strip.setPixelColor(boxstart, losingColor);
        strip.setPixelColor(boxstart+boxsize+1, losingColor);
      }

      if(gameStatus == notstarted && indicatorsWithinBounds()){
        gameStatus = started;
        strip.setPixelColor(boxstart, COLORGREEN);
        strip.setPixelColor(boxstart+boxsize+1, COLORGREEN);
      }
      if(gameStatus == notstarted && !indicatorsWithinBounds()){
        strip.setPixelColor(boxstart, COLORRED);
        strip.setPixelColor(boxstart+boxsize+1, COLORRED);
      }

      if(gameStatus == lost){
        strip.setPixelColor(boxstart, losingColor);
        strip.setPixelColor(boxstart+boxsize+1, losingColor);
      }

      strip.show();
      return;
    }
    void setConsumptionRate(float consrate) {
      consumptionRate = 1+consrate/100.;
      return;
    }
    void setIndicatorPosition(float position, int devnum) {
      if(devnum == HANDGRIPDEVNUM){
        strip.setPixelColor(prevSqueezeIndPos, 0,0,0);
        if(position < LED_COUNT && position >= 0){
          squeezeIndicatorPosition = position;
          strip.setPixelColor(position, SQUEEZEINDICATORCOLOR);
          prevSqueezeIndPos = (int)position;
        }
      }
      if(devnum == CRANKDEVNUM){
        strip.setPixelColor(prevCrankIndPos, 0,0,0);
        if(position < LED_COUNT && position >= 0){
          crankIndicatorPosition = position;
          strip.setPixelColor(position, CRANKINDICATORCOLOR);
          prevCrankIndPos = (int)position;
        }
      }
      return;
    }
    //used to set the production rate. 
    //prodrate: a number between 0 and 100.
    //the final set rate will be a number between 0 and 1. 
    float setProductionRate(float prodrate, int devnum) {
      if(devnum == HANDGRIPDEVNUM){
        squeezeProductionRate = prodrate/100.;
      }
      if(devnum == CRANKDEVNUM){
        crankProductionRate = prodrate/100.;
      }
      return;
    }
    float returnConsumptionRate() {
      return consumptionRate;
    }
    //this is used to set the rates of production and consumption for homeostasis. 
    // void setRates(float productionrate, float consumptionrate, int devnum); 
    void update(void) {
      strip.show();
      if(STRIPTESTMODE){
        if(HANDGRIPACTIVE == 1){
          Serial.print(" GRIP LED POSITION: ");
          
        }
        if(CRANKACTIVE == 1){
          Serial.print("CRANK LED POSITION: ");
          Serial.println(crankIndicatorPosition);
        }
      }
      return(0);
    }
  private:
    float consumptionRate = DEFAULTCONSUMPTIONRATE/100.; //stays the same for all players
    float crankProductionRate = DEFAULTPRODUCTIONRATE/100.;
    unsigned long losingColor;
    float squeezeProductionRate = DEFAULTPRODUCTIONRATE/100.;
};

class _lcd: public _device {
  public: 
    Adafruit_RGBLCDShield *lcd_ptr;
    _lcd(){
      lcd_ptr = new Adafruit_RGBLCDShield();
    }
};

class _handgrip: public _affector {
  public:
    _handgrip();
    float voltageValue(void) {
      return 5*analogRead(HANDGRIPPIN)/1023.; //simply returns the voltage read off the handgrip
    }
    //calculates the productionrate to feed into _indicatorstrip.setRate()
    //returns a value between 0 and HANDGRIPPRESCALER
    int calculateProductionRate(float pinADCval, _handgrip *Handgrip) {
      float voltagedelta = voltageValue()-Handgrip->handgripMinVoltage;
      float delta = (voltagedelta /(Handgrip->handgripMaxVoltage-Handgrip->handgripMinVoltage)) * 
        Handgrip->handgripPrescaler;
      return delta;
    }

      int calibrationState = 0; //standby state
      float handgripMinVoltage;
      float handgripMaxVoltage;
      float handgripPrescaler = 75;
      float productionRate = 0;
  private:
};

class _encoder: public _affector {
  public:

    //set up the pins ands store the current values for the encoder

    //check https://playground.arduino.cc/Main/RotaryEncoders/#OnInterrupts
    //TO DO: Change to pin change interrupts and do software debouncing. 
    void initialize(void){
      pinMode(ENCODERPINA, INPUT_PULLUP);
      pinMode(ENCODERPINB, INPUT_PULLUP);
      prevAVal = digitalRead(ENCODERPINA);
      prevBVal = digitalRead(ENCODERPINB);
    }
    //polls the encoder pins and evaluates if the encoder has moved forward, backward, or stayed the same. 
    //returns 0 if no change, 1 if a CW movement was detected, -1 if a CCW movement was detected. 
    //using this in conjuction with a low pass filter helps clean up switch bounce
    //http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html to see how it works
    char returnDelta(void){
      pinAVal = digitalRead(ENCODERPINA);
      pinBVal = digitalRead(ENCODERPINB);
      unsigned char lookupVal = (prevAVal << 3) | (prevBVal << 2) | (pinAVal << 1) | pinBVal;
      prevAVal = pinAVal;
      prevBVal = prevBVal;
      return quadratureLookupTable[lookupVal];
    }
    //we want to limit the crank to 1 RPM, as the encoder is only rated to that. It will
    //also make the device last longer as students wont be wild with it.
    //return a number between 0 and CRANKRATESCALER
    int calculateProductionRate(int crankSum){
      // calculates current moving average efficiently
      movingAverage += -movingAverage/movingAveragePeriod + crankSum;
      //make it pointless to spin the crank faster than the max spec RPM of 60
      //per the data sheet. It is a 24 position encoder and the moving average
        //is calculated over a period of 1 second. So we are trimming any pulses over 
        //24 in this function
      if(movingAverage > 24) movingAverage = 24;
      //prevent excessively small carryover
      if(movingAverage < 0.01) movingAverage = 0;
      // productionRate = movingAverage/24 * CRANKRATESCALER;
      return(movingAverage/24 * CRANKRATESCALER);
    }
    float productionRate = 0;
    int CRANKRATECALCDELAY 250 //value is in milliseconds
    int CRANKRATEMAX 24
    int CRANKRATESCALER 67
  private:
    volatile bool prevAVal;
    volatile bool prevBVal;
    volatile bool pinAVal;
    volatile bool pinBVal;
    float movingAverage = 0; //holds the moving average for the production of the hand crank. 
    float movingAveragePeriod = 1000/CRANKRATECALCDELAY; 
    // char quadratureLookupTable[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    //removed extraneous values to help prevent bouncing, and inverted the polarity
    char quadratureLookupTable[16] = {0,0,0,0,0,0,0,-1,0,0,0,0,0,1,0,0};
    // http://makeatronics.blogspot.com/2013/02/efficiently-reading-quadrature-with.html
};

//generic pointer declarations
_device *DCON1_ptr;
_device *ACON1_ptr;
_device *DACON1_ptr;
_device *DCON2_ptr;
_device *ACON2_ptr;
_device *DACON2_ptr;

// _device *Indicatorstrip_ptr = new _indicatorstrip; //object for the indicatorstrip
// _device *Handgrip_ptr = new _handgrip; //object for the handgrip
_device *Handcrank_ptr = new _encoder; //object for the encoder
// _device *menu_ptr = new _menu; //object for the menu system
_device *lcd_ptr = new _lcd;
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
  if(crankRateCalcDelayCounter >= (*Handcrank_ptr).CRANKRATECALCDELAY){
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
  // if(CRANKACTIVE == 1){
  //   int currentOut = (*Handcrank_ptr).returnDelta();
  //     //make sure it's not an invalid state change
  //     if(currentOut){ 
  //       //two or more matching values. Helps with logical debounce
  //       if(currentOut == prevOut){ 
  //         crankSum += currentOut;
  //       }
  //       prevOut = currentOut; //update the previous value
  //   }
  // }
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
  // (*Indicatorstrip_ptr).initialize();
  // (*Handcrank_ptr).initialize();

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
  INITIALSETUPFLAG = true;
}

void loop() {
  if(INITIALSETUPFLAG){
  //init LCD
    (*(_lcd*)(lcd_ptr)).lcd_ptr->begin(16, 2);
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
  // if(LCDREFRESHFLAG){
  //   // (note: line 1 is the second row, since counting begins with 0)
  //   uint8_t button = (*lcd_ptr).readButtons();
  //   if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == running){
  //     selectTimer++;
  //   }
  //   if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == running){
  //     systemMode = config;
  //     (*lcd_ptr).clear();
  //     (*menu_ptr).currentScreen = 0;
  //     (*menu_ptr).printMenu(lcd_ptr);

  //     (*lcd_ptr).setBacklight(RED);
  //     selectTimer = 0;
  //   }
  //   if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == config){
  //     selectTimer++;
  //   }
  //   if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == config){
  //     systemMode = running;
  //     (*lcd_ptr).setBacklight(WHITE);
  //     selectTimer = 0;
  //   }
  //   if(systemMode == running){
  //     (*lcd_ptr).clear();
  //     // lcd_ptr->setCursor(0,0);
  //     // lcd_ptr->print("System");
  //     // lcd_ptr->setCursor(0,1);
  //     // lcd_ptr->print("Operational");
  //   }
  //   if(systemMode == config){
  //     (*menu_ptr).navigateMenu(button, lcd_ptr);
  //   }
  //   // navigateMenu(button);
  // LCDREFRESHFLAG = false;
  // }
}
