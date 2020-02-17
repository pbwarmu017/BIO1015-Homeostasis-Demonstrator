/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/

//GLOBAL VARIABLES------------------------------------------
  int selectTimer = 0; //used for trackikng how long the select button has been depressed.
  bool configchange; //used to prompt changes in system config by user.
  uint8_t lastButton = 0;

//INCLUDES----------------------------------------
#include "superclasses.cpp"
#include "multi-effector_homeostasis_device.h"
#include "encoder.cpp"
#include "handgrip.cpp"
#include "lcd.cpp"
#include "indicatorstrip.cpp"
#include "menu.cpp"

//ENUM DEFINITIONS ------------------------------------------
  enum GAMESTATUS gameStatus = notstarted;
  enum SYSTEMMODE systemMode = config;

//GLOBAl VARIABLES FOR USE IN ISRS------------------------------------------
  volatile byte stripDelayCounter = 0; 
  volatile byte crankRateCalcDelayCounter = 0;
  volatile byte gripRateCalcDelayCounter = 0;
  volatile int gameResetCounter = 0;
  volatile byte lcdRefreshCounter = 0;

  volatile bool selectState = false; 
  volatile bool selectPrevState = false; 

  volatile bool STRIPREFRESHFLAG = false;
  volatile bool CRANKSUMFLAG = false;
  // volatile bool CRANKRATECALCFLAG = false;
  // volatile bool GRIPRATECALCFLAG = false;
  volatile bool LCDREFRESHFLAG = false;
  volatile bool RESETFLAG = false;

//GLOBAL OBJECTS------------------------------------------
  //generic pointer declarations. Specific device pointers will be assigned to these as needed by the system. 
  _device *DCON1_ptr;
  _device *ACON1_ptr;
  _device *DACON1_ptr;
  _device *DCON2_ptr;
  _device *ACON2_ptr;
  _device *DACON2_ptr;
  _device *main_ptr = new _device;
  _indicatorstrip *indicatorstrip_ptr;
  _menu *menu_ptr;
  _lcd *lcd_ptr;

//GLOBAL FUNCTIONS-------------------------------------------
  //this function is responsible for creating a new object on the HEAP (using the new keyword), setting up the ports 
  //for that object and then assigning it to the correct global object. 
  //types and portnums are definded in superclasses.cpp
  void createObject(int objtype, int portnum)
  {
    if(objtype == LCD_TYPE)
    {

      lcd_ptr = new _lcd;
    }

    if(objtype == MENU_TYPE)
    {

      menu_ptr = new _menu;
    }

    if(objtype == INDICATORSTRIP_TYPE)
    {

      indicatorstrip_ptr = new _indicatorstrip(menu_ptr); 
    }

    if(objtype == HANDGRIP_TYPE){
      //make sure that the port isnt active already
      if((portnum == ACON1_PORTNUM && !main_ptr->ACON1_mode) || (portnum == ACON2_PORTNUM && !main_ptr->ACON2_mode))
      {
        menu_ptr->objectCount++;
        if(portnum == ACON1_PORTNUM) ACON1_ptr = new _handgrip(ACON1_PORTNUM, main_ptr, 
          indicatorstrip_ptr, lcd_ptr, menu_ptr); 

        if(portnum == ACON2_PORTNUM) ACON2_ptr = new _handgrip(ACON2_PORTNUM, main_ptr, 
          indicatorstrip_ptr, lcd_ptr, menu_ptr);
      }
    }

    if(objtype == HANDCRANK_TYPE)
    {
      //make sure that the port isnt active already
      if((portnum == DCON1_PORTNUM && !main_ptr->DCON1_mode) || (portnum == DCON2_PORTNUM && !main_ptr->DCON2_mode))
      {
        menu_ptr->objectCount++;
        if(portnum == DCON1_PORTNUM) DCON1_ptr = new _encoder(DCON1_PORTNUM, main_ptr, 
          indicatorstrip_ptr, lcd_ptr, menu_ptr); 

        if(portnum == DCON2_PORTNUM) DCON2_ptr = new _encoder(DCON2_PORTNUM, main_ptr, 
          indicatorstrip_ptr, lcd_ptr, menu_ptr);
      }
    }
  }

  void deleteObject(int objtype, int portnum)
  {
    //make sure the port hasnt already been deactivated
    if(objtype == HANDGRIP_TYPE && (main_ptr->ACON1_mode || main_ptr->ACON2_mode))
    {
      menu_ptr->objectCount--;
      if(portnum == ACON1_PORTNUM) delete ACON1_ptr;

      if(portnum == ACON2_PORTNUM) delete ACON2_ptr;

      if(portnum == DACON1_PORTNUM) delete DACON1_ptr;

      if(portnum == DACON2_PORTNUM) delete DACON2_ptr;
    }
    //make sure the port hasnt already been deactivated
    if(objtype == HANDCRANK_TYPE && (main_ptr->DCON1_mode || main_ptr->DCON2_mode))
    {
      menu_ptr->objectCount--;
      if(portnum == DCON1_PORTNUM) delete DCON1_ptr;

      if(portnum == DCON2_PORTNUM) delete DCON2_ptr;
    }
  }

//INTERRUPT SERVICE ROUTINES------------------------------------------
  //Interrupt service routine for a timer that exectures every millisecond. DO NOT call functions/methods from within
  //an ISR. Set flags that are checked for within the main loop. You want to spend as little time inside of an ISR as
  //possible to prevent undefined program behavior.  
  ISR(TIMER0_COMPA_vect)
  {
    stripDelayCounter++;
    crankRateCalcDelayCounter++;
    gripRateCalcDelayCounter++;
    lcdRefreshCounter++;
    //set flags for strip
    if(stripDelayCounter >= STRIPREFRESHDELAY)
    { 
      stripDelayCounter = 0; //reset the timer counter for the next run.
      STRIPREFRESHFLAG = true;
      //Set Rates based on affector positions (one for each affector)
    }
    //set flags to reset lost game
    if(gameStatus == lost)
    {
      gameResetCounter++;
      if(gameResetCounter >= GAMERESETDELAY)
      {
        gameResetCounter = 0;
        RESETFLAG = true;

      }
    }
    //set flags to refresh LCD
    if(lcdRefreshCounter >= LCDREFRESHDELAY)
    {
      LCDREFRESHFLAG = true;  
      lcdRefreshCounter = 0;  
    }
  }

  //Interrupt service routine for pin change interrupt on D0 to D7. DO NOT call functions/methods from within
  //an ISR. Set flags that are checked for within the main loop. You want to spend as little time inside of an ISR as
  //possible to prevent undefined program behavior.
  ISR(PCINT2_vect)
  { // handle pin change interrupt for D0 to D7 here
    if(main_ptr->DCON1_mode == HANDCRANK_TYPE or main_ptr->DCON2_mode == HANDCRANK_TYPE) CRANKSUMFLAG = true;
  }

  //Interrupt service routine for pin change interrupt on D8 to D13. DO NOT call functions/methods from within
  //an ISR. Set flags that are checked for within the main loop. You want to spend as little time inside of an ISR as
  //possible to prevent undefined program behavior.
  ISR(PCINT0_vect)
  {
    if(main_ptr->DCON1_mode == HANDCRANK_TYPE or main_ptr->DCON2_mode == HANDCRANK_TYPE) CRANKSUMFLAG = true;
  }

//MAIN ARDINO FUNCTIONS------------------------------------------
  void setup()
  {

    //init serial for debugging  
    Serial.begin(2000000); 

    //setup timer0 to call interrupt OCR0A every millisecond
    OCR0A = 0xFA; //set to trigger TIMER0_COMPA_vect every millisecond
    TIMSK0 |= _BV(OCIE0A); //enable the output compare interrupt on timer0

    //make all digital pins float high to prevent pin change interrupt on stray voltages
    //these will be individually modified as needed by objects. 
    pinMode(0, INPUT_PULLUP);
    pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);
    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);

    //----------------------------------------------------------
    createObject(LCD_TYPE, HARDCODED_PORTNUM);
    createObject(MENU_TYPE, HARDCODED_PORTNUM);
    createObject(INDICATORSTRIP_TYPE, HARDCODED_PORTNUM);
    //here to catch when the sytem mode goes to config by default. 
    if(systemMode == config) menu_ptr->printMenu(lcd_ptr);
  }

  void loop()
  {
    if(CRANKSUMFLAG) //triggers calcultion of the moving average for the handcrank
    {
      if(main_ptr->DCON1_mode == HANDCRANK_TYPE)
      {
        DCON1_ptr->calculateRate(CRANKSUM_RATETYPE);
      }
      if(main_ptr->DCON2_mode == HANDCRANK_TYPE)
      {
        DCON2_ptr->calculateRate(CRANKSUM_RATETYPE);
      }
      CRANKSUMFLAG = false;
    } 
    if(STRIPREFRESHFLAG) //triggers rate calculation for active affectros and strip refresh
    {
      if(main_ptr->DCON1_mode == HANDCRANK_TYPE)
      {
        DCON1_ptr->calculateRate(GENERAL_RATETYPE);
      }
      if(main_ptr->DCON2_mode == HANDCRANK_TYPE)
      {
        DCON2_ptr->calculateRate(GENERAL_RATETYPE);
      }
      if(main_ptr->ACON1_mode == HANDGRIP_TYPE)
      {
        ACON1_ptr->calculateRate(GENERAL_RATETYPE);
      }
      if(main_ptr->ACON2_mode == HANDGRIP_TYPE)
      {
        ACON2_ptr->calculateRate(GENERAL_RATETYPE);
      }
      indicatorstrip_ptr->refreshStrip();
      STRIPREFRESHFLAG = false;
    }
    if(RESETFLAG)
    {
      gameStatus = notstarted;
      RESETFLAG = false;
    }
    if(LCDREFRESHFLAG)
    {
      Serial.print(menu_ptr->objectCount);
      Serial.print("\n");
    uint8_t button = (lcd_ptr->lcd_obj)->readButtons();
     //track the states of the button
      selectPrevState = selectState;
      //if the select button is pressed down
      if(button & BUTTON_SELECT)
      {
        selectState = true;
      }
      else //if its not pressed down
      {
        selectState = false;
      }

      //if the state of the select button is true
      if(selectState == true)
      {
        if(selectPrevState == true)
        {
          selectTimer++;
        }
      }
      else
      {
        selectTimer = 0;
      }

      if(selectState == false && selectPrevState == true)
      {
        if(systemMode == config)
        {
          menu_ptr->navigateMenu(BUTTON_SELECT, lcd_ptr);
        }
      }
      //currently not needed, but the capability is there to have two different menus

/*      if(selectTimer == 10 && systemMode == running)
      {
        systemMode = config;
        (lcd_ptr->lcd_obj)->clear();
        menu_ptr->printMenu(lcd_ptr);
        selectTimer++;

        // (lcd_ptr->lcd_obj)->setBacklight(RED);
      }

      if(selectTimer == 10 && systemMode == config)
      {
        systemMode = running;
        (lcd_ptr->lcd_obj)->clear();
        selectTimer++;
        // (lcd_ptr->lcd_obj)->setBacklight(WHITE);
      }*/

    if(button && !(button & BUTTON_SELECT) && systemMode == config && lastButton != button)
    {
      menu_ptr->navigateMenu(button, lcd_ptr);
    }

    LCDREFRESHFLAG = false;
    lastButton = button;
  }
}
