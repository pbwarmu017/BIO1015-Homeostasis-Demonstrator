/*
REQUIRED LIBRARIES: Must be installed in the arduino IDE to compile this
Adafruit_Neopixel
Adafruit RGB LCD Sheild Library
*/

//GLOBAL VARIABLES------------------------------------------
  int selectTimer = 0; //used for trackikng how long the select button has been depressed.
  bool configchange; //used to prompt changes in system config by user.

//INCLUDES ------------------------------------------
  #include "superclasses.cpp"
  #include "encoder.cpp"
  #include "handgrip.cpp"
  #include "lcd.cpp"
  #include "indicatorstrip.cpp"
  #include "menu.cpp"

//ENUM DEFINITIONS ------------------------------------------
  enum GAMESTATUS gameStatus = notstarted;
  enum SYSTEMMODE systemMode = running;

//GLOBAl VARIABLES FOR USE IN ISRS------------------------------------------
  volatile unsigned stripDelayCounter = 0; 
  volatile int prevOut = 0;
  volatile int crankRateCalcDelayCounter = 50;
  volatile int gameResetCounter = 0;
  volatile int lcdRefreshCounter = 0;
  volatile bool STRIPREFRESHFLAG = false;
  volatile bool CRANKSUMFLAG = false;
  volatile bool CRANKRATECALCFLAG = false;
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
  _device *indicatorstrip_ptr;
  _device *menu_ptr;
  _device *lcd_ptr;
  _device *main_ptr = new _device;

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

      indicatorstrip_ptr = new _indicatorstrip; 
    }

    if(objtype == HANDGRIP_TYPE){
      if(portnum == ACON1_PORTNUM) ACON1_ptr = new _handgrip(ACON1_PORTNUM); 

      if(portnum == ACON2_PORTNUM) ACON2_ptr = new _handgrip(ACON2_PORTNUM);

      if(portnum == DACON1_PORTNUM) DACON1_ptr = new _handgrip(DACON1_PORTNUM);

      if(portnum == DACON2_PORTNUM) DACON2_ptr = new _handgrip(DACON2_PORTNUM);
    }

    if(objtype == ENCODER_TYPE)
    {
  //     // gen_ptr = new _encoder((_indicatorstrip*)indicatorstrip_ptr);

  //     if(portnum == DCON1_PORTNUM){
  //       ((_encoder*)gen_ptr)->encoderpina = 3;
  //       ((_encoder*)gen_ptr)->encoderpinb = 5;
  //     }
  //     if(portnum == DCON2_PORTNUM){
  //       ((_encoder*)gen_ptr)->encoderpina = 9;
  //       ((_encoder*)gen_ptr)->encoderpinb = 10;
  //     }
  //     // enable pin change interrupt for encoder pin A
  //     *digitalPinToPCMSK(((_encoder*)gen_ptr)->encoderpina) |= 
  //       bit(digitalPinToPCMSKbit(((_encoder*)gen_ptr)->encoderpina)); 

  //     // enable ping change interrupt for encoder pin B
  //     *digitalPinToPCMSK(((_encoder*)gen_ptr)->encoderpinb) |= 
  //       bit(digitalPinToPCMSKbit(((_encoder*)gen_ptr)->encoderpinb)); 

  //     // clear any outstanding pin change interrupt flags
  //     PCIFR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpina)); 
  //     PCIFR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpinb));

  //     // enable interrupt for the GROUP (digital pins 1-7, digtial pins 8-13)
  //     PCICR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpina)); 
  //     PCICR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpinb));

  //     pinMode(((_encoder*)gen_ptr)->encoderpinb, INPUT_PULLUP);
  //     pinMode(((_encoder*)gen_ptr)->encoderpina, INPUT_PULLUP);
    }
  }

  void deleteObject(int objtype, int portnum)
  {
    if(objtype == HANDGRIP_TYPE)
    {
      if(portnum == ACON1_PORTNUM) delete ACON1_ptr;

      if(portnum == ACON2_PORTNUM) delete ACON2_ptr;

      if(portnum == DACON1_PORTNUM) delete DACON1_ptr;

      if(portnum == DACON2_PORTNUM) delete DACON2_ptr;
    }

    if(objtype == ENCODER_TYPE)
    {
      Serial.print("(Funmction) Encoder Deleted, Port ");
      Serial.print(portnum);
      Serial.print("\n");
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
    lcdRefreshCounter++;
    if(stripDelayCounter >= STRIPREFRESHDELAY)
    { 
      stripDelayCounter = 0; //reset the timer counter for the next run.
      STRIPREFRESHFLAG = true;
      //Set Rates based on affector positions (one for each affector)
    }
    if(crankRateCalcDelayCounter >= CRANKRATECALCDELAY)
    {
      crankRateCalcDelayCounter = 0; 
      CRANKRATECALCFLAG = true;
      
    }

    if(gameStatus == lost)
    {
      gameResetCounter++;
      if(gameResetCounter >= GAMERESETDELAY)
      {
        gameResetCounter = 0;
        gameStatus = notstarted;
        RESETFLAG = true;

      }
    }

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
    // if(CRANKACTIVE == 1){
    //   int currentOut = ((_encoder  *)Handcrank_ptr)->returnDelta();
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
    // //set all analog ports as inputs. 
    // pinMode(A0, INPUT);
    // pinMode(A1, INPUT);
    // pinMode(A2, INPUT);
    // pinMode(A3, INPUT);

    //----------------------------------------------------------
    createObject(LCD_TYPE, HARDCODED_PORTNUM);
    createObject(MENU_TYPE, HARDCODED_PORTNUM);
    createObject(INDICATORSTRIP_TYPE, HARDCODED_PORTNUM);
  }

  void loop()
  {
    // if(STRIPREFRESHFLAG){
    //   if(HANDGRIPACTIVE == 1){
    //     if->Handgrip_ptrcalibrationState == true){
    //      ->indicatorstrip_ptrsetProductionRate->Handgrip_ptrcalculateProductionRate(
    //         analogRead(HANDGRIPPIN), Handgrip_ptr), HANDGRIPDEVNUM);
    //       //set the indicator positions based on the production rate 
    //      ->indicatorstrip_ptrsetIndicatorPosition(
    //        ->Handgrip_ptrproductionRate =->indicatorstrip_ptrcalculatePosition(HANDGRIPDEVNUM), HANDGRIPDEVNUM);
    //     } else {
    //      ->Handgrip_ptrhandgripMaxVoltage =->Handgrip_ptrvoltageValue();
    //      ->Handgrip_ptrcalibrationState = true;
    //     }
    //   }
    //   //set the bounding box. 
    //   if(CRANKACTIVE == 1){
    //    ->indicatorstrip_ptrsetIndicatorPosition(
    //      ->Handcrank_ptrproductionRate =->indicatorstrip_ptrcalculatePosition(CRANKDEVNUM), CRANKDEVNUM);
    //   }
    //  ->indicatorstrip_ptrsetBoundingBox(BOXSTART, BOXSIZE);
    //  ->indicatorstrip_ptrupdate();
    //   STRIPREFRESHFLAG = false;
    // }
    // if(CRANKRATECALCFLAG){
    //   if(CRANKACTIVE == 1){
    //    ->indicatorstrip_ptrsetProductionRate->Handcrank_ptrcalculateProductionRate(
    //       crankSum), CRANKDEVNUM);
    //     //reset the sum because it has just been incorporated into a moving avg
    //     crankSum = 0; 
    //     //reset the delay counter for the next run
    //     CRANKRATECALCFLAG = false;
    //   }
    // }
    // if(RESETFLAG){
    //  ->indicatorstrip_ptrsetBoundingBox(BOXSTART, BOXSIZE);
    //   RESETFLAG = false;
    // }
    if(LCDREFRESHFLAG)
    {
      // (note: line 1 is the second row, since counting begins with 0)
      uint8_t button = (((_lcd *)lcd_ptr)->lcd_obj)->readButtons();
      if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == running)
      {
        selectTimer++;
      }

      if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == running)
      {
        systemMode = config;
        (((_lcd *)lcd_ptr)->lcd_obj)->clear();
        ((_menu *)menu_ptr)->currentScreen = 6;
        ((_menu *)menu_ptr)->printMenu( (_lcd*)lcd_ptr );

        (((_lcd *)lcd_ptr)->lcd_obj)->setBacklight(RED);
        selectTimer = 0;
      }

      if(button & BUTTON_SELECT && selectTimer <= 10 && systemMode == config)
      {
        selectTimer++;
      }

      if(button & BUTTON_SELECT && selectTimer > 10 && systemMode == config)
      {
        systemMode = running;
        (((_lcd *)lcd_ptr)->lcd_obj)->setBacklight(WHITE);
        selectTimer = 0;
      }

      if(systemMode == running)
      {
        (((_lcd *)lcd_ptr)->lcd_obj)->clear();
      }

      if(systemMode == config)
      {
        ((_menu *)menu_ptr)->navigateMenu(button, (_lcd*)lcd_ptr);
      }
    LCDREFRESHFLAG = false;
    }
  }
