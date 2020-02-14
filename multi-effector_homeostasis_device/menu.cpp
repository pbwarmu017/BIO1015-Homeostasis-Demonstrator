#ifndef MENU_CPP
#define MENU_CPP

#include "superclasses.cpp"
#include "lcd.cpp"
#include "multi-effector_homeostasis_device.h"

//DEFINES

  //DEVICE STATES (used to track port status. As you add affectors and use these states, add descriptions here)
  //note: there can only be 16 port states, as I am using only 4 bits to track it. 
  #define DCONSTATEOFF 0 //OFF
  #define DCONSTATEHANDCRANK 1 //HANDCRANK
  // #define DCONSTATE2 2
  // #define DCONSTATE3 3
  // #define DCONSTATE4 4
  // #define DCONSTATE5 5
  // #define DCONSTATE6 6
  // #define DCONSTATE7 7
  // #define DCONSTATE8 8
  // #define DCONSTATE9 9
  // #define DCONSTATE10 10
  // #define DCONSTATE11 11
  // #define DCONSTATE12 12
  // #define DCONSTATE13 13
  // #define DCONSTATE14 14
  // #define DCONSTATE15 15


  #define ACONSTATEOFF 0 //OFF
  #define ACONSTATEHANDGRIP 1 //HANDGRIP
  // #define ACONSTATE2 2
  // #define ACONSTATE3 3
  // #define ACONSTATE4 4
  // #define ACONSTATE5 5
  // #define ACONSTATE6 6
  // #define ACONSTATE7 7
  // #define ACONSTATE8 8
  // #define ACONSTATE9 9
  // #define ACONSTATE10 10
  // #define ACONSTATE11 11
  // #define ACONSTATE12 12
  // #define ACONSTATE13 13
  // #define ACONSTATE14 14
  // #define ACONSTATEU15 15

  #define DACONSTATEOFF 0 //OFF
  #define DACONSTATE1 1
  // #define DACONSTATE2 2
  // #define DACONSTATE3 3
  // #define DACONSTATE4 4
  // #define DACONSTATE5 5
  // #define DACONSTATE6 6
  // #define DACONSTATE7 7
  // #define DACONSTATE8 8
  // #define DACONSTATE9 9
  // #define DACONSTATE10 10
  // #define DACONSTATE11 11
  // #define DACONSTATE12 12
  // #define DACONSTATE13 13
  // #define DACONSTATE14 14
  // #define DACONSTATE15 15

  #define BOUNDINGBOXSTATIONARY 0
  #define BOUNDINGBOXMOVING 1

  #define DCON1FLAG 0
  #define DCON2FLAG 1
  #define ACON1FLAG 2
  #define ACON2FLAG 3
  #define DACON1FLAG 4
  #define DACON2FLAG 5

//LCD MENU TEXT OPTIONS
  //this is some PROGMEM magic to move the strings to program memory and free up SRAM

  //Main Menu Screens (line 1)
  #define NUMOFSCREENS 9 //number of options in screens[]
  constexpr const char screen_1_1[17] PROGMEM = "PORT DCON1- WHTE";
  constexpr const char screen_2_1[17] PROGMEM = "PORT ACON1- ORNG";
  constexpr const char screen_3_1[17] PROGMEM = "PORT DACON1- YEL";
  constexpr const char screen_4_1[17] PROGMEM = "PORT DCON2- BLUE";
  constexpr const char screen_5_1[17] PROGMEM = "PORT ACON2- CYAN";
  constexpr const char screen_6_1[17] PROGMEM = "PORT DACON2-VIOL";
  constexpr const char screen_7_1[17] PROGMEM = "BOUNDING BOX";
  constexpr const char screen_8_1[17] PROGMEM = "BOUNDING BOX";
  constexpr const char screen_9_1[17] PROGMEM = "SOFTWARE VERSION";
  constexpr const char* screen1table[NUMOFSCREENS] PROGMEM = {screen_1_1, screen_2_1, screen_3_1, screen_4_1, 
    screen_5_1, screen_6_1, screen_7_1, screen_8_1, screen_9_1};

  //Main Menu Screens (line 2)
  constexpr const char screen_1_2[17] PROGMEM = "DEV: ";
  constexpr const char screen_2_2[17] PROGMEM = "DEV: ";
  constexpr const char screen_3_2[17] PROGMEM = "DEV: ";
  constexpr const char screen_4_2[17] PROGMEM = "DEV: ";
  constexpr const char screen_5_2[17] PROGMEM = "DEV: ";
  constexpr const char screen_6_2[17] PROGMEM = "DEV: ";
  constexpr const char screen_7_2[17] PROGMEM = "MODE: ";
  constexpr const char screen_8_2[17] PROGMEM = "SIZE: ";
  constexpr const char screen_9_2[17] PROGMEM = "";
  constexpr const char* screen2table[] PROGMEM =  {screen_1_2, screen_2_2, screen_3_2, screen_4_2, screen_5_2, 
    screen_6_2, screen_7_2, screen_8_2, screen_9_2};

  //DCON device options
  #define NUMOFDCONDEVS 2 //number of options in dconscreens[]
  constexpr const char dconscreen_1[17] PROGMEM = {"OFF"};
  constexpr const char dconscreen_2[17] PROGMEM = {"HAND CRANK"};
  constexpr const char* dconscreentable[] PROGMEM = {dconscreen_1, dconscreen_2};

  //ACON device options
  #define NUMOFACONDEVS 2 //number of options in aconscreens[]
  constexpr const char aconscreen_1[17] PROGMEM = {"OFF"};
  constexpr const char aconscreen_2[17] PROGMEM = {"SQZE GRIP"};
  constexpr const char* aconscreentable[] PROGMEM = {aconscreen_1, aconscreen_2};

  //DACON device options
  #define NUMOFDACONDEVS 1 //number of options in daconscreens[]
  constexpr const char daconscreen_1[17] PROGMEM = {"N/A"};
  constexpr const char* daconscreentable[] PROGMEM = {daconscreen_1};

  //BOUNDING BOX MODE OPTIONS
  #define NUMOFBOUNDINGBOXMODES 5 //number of options in boundingBoxModeScreens
  constexpr const char boundingBoxModeScreens_1[17] PROGMEM = {"FROZEN"};
  constexpr const char boundingBoxModeScreens_2[17] PROGMEM = {"MOVING 1X"};
  constexpr const char boundingBoxModeScreens_3[17] PROGMEM = {"MOVING 2X"};
  constexpr const char boundingBoxModeScreens_4[17] PROGMEM = {"MOVING 3X"};
  constexpr const char boundingBoxModeScreens_5[17] PROGMEM = {"MOVING 4X"};
  constexpr const char* boundingBoxModeScreensTable[] PROGMEM = {boundingBoxModeScreens_1, boundingBoxModeScreens_2, 
    boundingBoxModeScreens_3, boundingBoxModeScreens_4, boundingBoxModeScreens_5};

class _menu: public _device{
  public:

    //mode tracking variables. Being as I have limited the number of available modes to 16 per port, 
    //that meeant that I could fit two different 4 bit values into an 8 bit unsigned integer. 
    //these values are modified and read by the modeselection functions below
      uint8_t objectCount = 0;
      uint8_t currentScreen = 0;

      //each of these are being split into two 4-bit numbers for tracking status info to save SRAM. 
      //This means that you can have up to 16 different states per each port. 
      //the selectedmode variables track the menu selection and the activemode variables track the 
      //current configuration of the ports. 
      uint8_t dconselectedmode = 0;
      uint8_t dconactivemode = 0;

      uint8_t aconselectedmode = 0;
      uint8_t aconactivemode = 0;

      uint8_t daconselectedmode = 0;
      uint8_t daconactivemode = 0;

      int8_t boundingboxmode = 0;
      uint8_t boundingboxsize = 10;

  //MODE SELECTION FUNCTIONS
    //returns a 4 bit value from one of the "selectedmode" variables above depending on the port specificed
    uint8_t getSelectedMode(const uint8_t port)
    {
      switch(port)
      {
        case DCON1_PORTNUM:
        return((dconselectedmode >> 4) & 0b00001111);
        break;

        case DCON2_PORTNUM:
        return((dconselectedmode) & 0b00001111);
        break;

        case ACON1_PORTNUM:
        return((aconselectedmode >> 4) & 0b00001111);
        break;

        case ACON2_PORTNUM:
        return((aconselectedmode) & 0b00001111);
        break;

        case DACON1_PORTNUM:
        return((daconselectedmode >> 4) & 0b00001111);
        break;

        case DACON2_PORTNUM:
        return((daconselectedmode) & 0b00001111);
        break;
      }
    }
    //sets a 4 bit value in one of the "selectedmode" variables above depending on the port specificed
    void setSelectedMode(const uint8_t port, uint8_t val)
    {
      switch(port)
      {
        case DCON1_PORTNUM:
        dconselectedmode &= 0b00001111; //clear the first four bits
        dconselectedmode |= (val << 4); //set the first four bits to val
        break;

        case DCON2_PORTNUM:
        dconselectedmode &= 0b11110000; //clear the last four bits
        dconselectedmode |= (val); //set the last four bits to val
        break;
        case ACON1_PORTNUM:
        aconselectedmode &= 0b00001111; //clear the first four bits
        aconselectedmode |= (val << 4); //set the first four bits to val
        break;

        case ACON2_PORTNUM:
        aconselectedmode &= 0b11110000; //clear the first four bits
        aconselectedmode |= (val); //set the first four bits to val
        break;
        case DACON1_PORTNUM:
        daconselectedmode &= 0b00001111; //clear the first four bits
        daconselectedmode |= (val << 4);//set the first four bits to val
        break;

        case DACON2_PORTNUM:
        daconselectedmode &= 0b11110000; //clear the first four bits
        daconselectedmode |= (val); //set the first four bits to val
        break;
      }
    }
    //returns a 4 bit value from one of the "activemode" variables above depending on the port specificed
    uint8_t getActiveMode(const uint8_t port)
    {
      switch(port)
      {
        case DCON1_PORTNUM:
        return((dconactivemode >> 4) & 0b00001111);
        break;

        case DCON2_PORTNUM:
        return((dconactivemode) & 0b00001111);
        break;

        case ACON1_PORTNUM:
        return((aconactivemode >> 4) & 0b00001111);
        break;

        case ACON2_PORTNUM:
        return((aconactivemode) & 0b00001111);
        break;

        case DACON1_PORTNUM:
        return((daconactivemode >> 4) & 0b00001111);
        break;

        case DACON2_PORTNUM:
        return(daconactivemode & 0b00001111);
        break;
      }
    }
    //sets a 4 bit value in one of the "activemode" variables above depending on the port specificed
    void setActiveMode(const uint8_t port, uint8_t val)
    {

      switch(port)
      {
        case DCON1_PORTNUM:
        dconactivemode &= 0b00001111; //clear the first four bits
        dconactivemode |= (val << 4); //set the first four bits to val

        break;

        case DCON2_PORTNUM:
        dconactivemode &= 0b11110000; //clear the last four bits
        dconactivemode |= (val); //set the last four bits to val
        break;
        case ACON1_PORTNUM:
        aconactivemode &= 0b00001111; //clear the first four bits
        aconactivemode |= (val << 4); //set the first four bits to val
        break;

        case ACON2_PORTNUM:
        aconactivemode &= 0b11110000; //clear the first four bits
        aconactivemode |= (val); //set the first four bits to val
        break;
        case DACON1_PORTNUM:
        daconactivemode &= 0b00001111; //clear the first four bits
        daconactivemode |= (val << 4);//set the first four bits to val
        break;

        case DACON2_PORTNUM:
        daconactivemode &= 0b11110000; //clear the first four bits
        daconactivemode |= (val); //set the first four bits to val
        break;
      }
    }
    //Handles what to do with a button press. 
    void navigateMenu(uint8_t button, _lcd* lcd_ptr)
    {
      if(button & BUTTON_UP) 
      {
        setSelectedMode(DCON1_PORTNUM, getActiveMode(DCON1_PORTNUM));
        setSelectedMode(ACON1_PORTNUM, getActiveMode(ACON1_PORTNUM));
        setSelectedMode(DACON1_PORTNUM, getActiveMode(DACON1_PORTNUM));
        setSelectedMode(DCON2_PORTNUM, getActiveMode(DCON2_PORTNUM));
        setSelectedMode(ACON2_PORTNUM, getActiveMode(ACON2_PORTNUM));
        setSelectedMode(DACON2_PORTNUM, getActiveMode(DACON2_PORTNUM));
        if (currentScreen == 0) 
        {
          currentScreen = NUMOFSCREENS-1;
          printMenu(lcd_ptr);
        }
        else
        {
          currentScreen--;
          printMenu(lcd_ptr);
        }
      }

      if(button & BUTTON_DOWN)
      {
        setSelectedMode(DCON1_PORTNUM, getActiveMode(DCON1_PORTNUM));
        setSelectedMode(ACON1_PORTNUM, getActiveMode(ACON1_PORTNUM));
        setSelectedMode(DACON1_PORTNUM, getActiveMode(DACON1_PORTNUM));
        setSelectedMode(DCON2_PORTNUM, getActiveMode(DCON2_PORTNUM));
        setSelectedMode(ACON2_PORTNUM, getActiveMode(ACON2_PORTNUM));
        setSelectedMode(DACON2_PORTNUM, getActiveMode(DACON2_PORTNUM));
        if (currentScreen >= NUMOFSCREENS-1) 
        {
          currentScreen = 0;
          printMenu(lcd_ptr);
        }
        else
        {
          currentScreen++;
          printMenu(lcd_ptr);
        }
      }

      if(button & BUTTON_RIGHT or button & BUTTON_LEFT) 
      {
        parameterChange(button, lcd_ptr);
        printMenu(lcd_ptr);
      }

      if(button & BUTTON_SELECT)
      {
        //checks each ports modes to see if selecttedmode is different than activemode
        //calls applyparameter() if there is a mismatch. 
        for(int i = 0; i < 6; i++)
        {
          if(getSelectedMode(i) != getActiveMode(i)) applyParameter(lcd_ptr);
        }
      }
    }
    //sets the activemode variable to the selected mode variable depending on the active menu screen, 
    //and calls for the creation or destruction of objects as needed. 
    void applyParameter(_lcd * lcd_ptr)
    {
      if(currentScreen == 0) 
      {//DCON1
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          if(getSelectedMode(DCON1FLAG) == DCONSTATEOFF)
          {
            setActiveMode(DCON1FLAG, DCONSTATEOFF);
            printMenu(lcd_ptr);

          }
          else
          {
            setSelectedMode(DCON1FLAG, getActiveMode(DCON1FLAG)); //reset the menu selection
            issueSizeWarning(lcd_ptr);
            printMenu(lcd_ptr);
          }
        }
        else //we still have rooom
        {
          if(getActiveMode(DCON1FLAG) == DCONSTATEHANDCRANK)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON1_PORTNUM);
          }
          if(getSelectedMode(DCON1FLAG) == DCONSTATEOFF)
          {
            setActiveMode(DCON1FLAG, DCONSTATEOFF);
            printMenu(lcd_ptr);
          }

          if(getSelectedMode(DCON1FLAG) == DCONSTATEHANDCRANK) 
          {
            setActiveMode(DCON1FLAG, DCONSTATEHANDCRANK);
            printMenu(lcd_ptr);
            createObject(HANDCRANK_TYPE, DCON1_PORTNUM);
          }
        }
      }
      if(currentScreen == 1)
      { //ACON1
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          if(getSelectedMode(ACON1FLAG) == ACONSTATEOFF)
          {
            setActiveMode(ACON1FLAG, ACONSTATEOFF);
            printMenu(lcd_ptr);

          }
          else
          {
            setSelectedMode(ACON1FLAG, getActiveMode(ACON1FLAG)); //reset the menu selection
            issueSizeWarning(lcd_ptr);
            printMenu(lcd_ptr);
          }
        }
        else //we still have rooom
        {
          if(getActiveMode(ACON1FLAG) == ACONSTATEHANDGRIP)
          { //if there has been an encoder configured previously
            deleteObject(HANDGRIP_TYPE, ACON1_PORTNUM);
          }
          if(getSelectedMode(ACON1FLAG) == ACONSTATEOFF)
          {
            setActiveMode(ACON1FLAG, ACONSTATEOFF);
            printMenu(lcd_ptr);
          }

          if(getSelectedMode(ACON1FLAG) == ACONSTATEHANDGRIP) 
          {
            setActiveMode(ACON1FLAG, ACONSTATEHANDGRIP);
            printMenu(lcd_ptr);
            createObject(HANDGRIP_TYPE, ACON1_PORTNUM);
          }
        }
      }
      if(currentScreen == 2)
      { //DACON1

        //there is nothing to apply here, yet. 
      } 
      if(currentScreen == 3) 
      {//DCON2
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          if(getSelectedMode(DCON2_PORTNUM) == DCONSTATEOFF)
          {
            setActiveMode(DCON2_PORTNUM, DCONSTATEOFF);
            deleteObject(HANDCRANK_TYPE, DCON2_PORTNUM);
            printMenu(lcd_ptr);

          }
          else
          {
            setSelectedMode(DCON2_PORTNUM, getActiveMode(DCON2_PORTNUM)); //reset the menu selection
            issueWarning(lcd_ptr, WARNINGTYPE_NUMDEVS);
            printMenu(lcd_ptr);
          }
        }
        else //we still have rooom
        {
          if(getActiveMode(DCON2FLAG) == DCONSTATEHANDCRANK)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON2_PORTNUM);
          }
          if(getSelectedMode(DCON2FLAG) == DCONSTATEOFF)
          {
            setActiveMode(DCON2FLAG, DCONSTATEOFF);
            printMenu(lcd_ptr);
          }

          if(getSelectedMode(DCON2FLAG) == DCONSTATEHANDCRANK) 
          {
            setActiveMode(DCON2FLAG, DCONSTATEHANDCRANK);
            printMenu(lcd_ptr);
            createObject(HANDCRANK_TYPE, DCON2_PORTNUM);
          }
        }
      }
      if(currentScreen == 4) 
      {//ACON2
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          if(getSelectedMode(ACON2FLAG) == ACONSTATEOFF)
          {
            setActiveMode(ACON2FLAG, ACONSTATEOFF);
            printMenu(lcd_ptr);

          }
          else
          {
            setSelectedMode(ACON2FLAG, getActiveMode(ACON2FLAG)); //reset the menu selection
            issueSizeWarning(lcd_ptr);
            printMenu(lcd_ptr);
          }
        }
        else //we still have rooom
        {
          if(getActiveMode(ACON2FLAG) == ACONSTATEHANDGRIP)
          { //if there has been an encoder configured previously
            deleteObject(HANDGRIP_TYPE, ACON2_PORTNUM);
          }
          if(getSelectedMode(ACON2FLAG) == ACONSTATEOFF)
          {
            setActiveMode(ACON2FLAG, ACONSTATEOFF);
            printMenu(lcd_ptr);
          }

          if(getSelectedMode(ACON2FLAG) == ACONSTATEHANDGRIP) 
          {
            setActiveMode(ACON2FLAG, ACONSTATEHANDGRIP);
            printMenu(lcd_ptr);
            createObject(HANDGRIP_TYPE, ACON2_PORTNUM);
          }
        }
      }
      if(currentScreen == 5)
      {//DACON2
        
        //nothing in here to do, yet
      }
    }

    //used to change the selectedmode variable for the selcted port when the left or right arrow button is pressed. 
    void parameterChange(uint8_t button, _lcd* lcd_ptr)
    {
      if(button & BUTTON_RIGHT)
      {
        if(currentScreen == 0)
        { //DCON1
          if(getSelectedMode(DCON1_PORTNUM) < NUMOFDCONDEVS-1)
          {
            uint8_t mode = getSelectedMode(DCON1_PORTNUM);
            setSelectedMode(DCON1_PORTNUM, ++mode);

          }
          else
          {
            setSelectedMode(DCON1_PORTNUM, DCONSTATEOFF);
          }
        }
        if(currentScreen == 1)
        { //ACON1
          if(getSelectedMode(ACON1_PORTNUM) < NUMOFACONDEVS-1)
          {
            uint8_t mode = getSelectedMode(ACON1_PORTNUM);
            setSelectedMode(ACON1_PORTNUM, ++mode);
          }
          else
          {
            setSelectedMode(ACON1_PORTNUM, ACONSTATEOFF);
          }
        }
        if(currentScreen == 2)
        { //DACON1
          if(getSelectedMode(DACON1_PORTNUM) < NUMOFDACONDEVS-1)
          {
            uint8_t mode = getSelectedMode(DACON1_PORTNUM);
            setSelectedMode(DACON1_PORTNUM, ++mode);
          }
          else
          {
            setSelectedMode(DACON1_PORTNUM, DACONSTATEOFF);
          }
        }
        if(currentScreen == 3)
        { //DCON2
          if(getSelectedMode(DCON2_PORTNUM) < NUMOFDCONDEVS-1)
          {
            uint8_t mode = getSelectedMode(DCON2_PORTNUM);
            setSelectedMode(DCON2_PORTNUM, ++mode);
          }
          else
          {
            setSelectedMode(DCON2_PORTNUM, DCONSTATEOFF);
          } 
        }
        if(currentScreen == 4)
        { //ACON2
          
          if(getSelectedMode(ACON2_PORTNUM) < NUMOFACONDEVS-1)
          {
            uint8_t mode = getSelectedMode(ACON2_PORTNUM);
            setSelectedMode(ACON2_PORTNUM, ++mode);
          }
          else
          {
            setSelectedMode(ACON2_PORTNUM, ACONSTATEOFF);
          }
        }
        if(currentScreen == 5)
        { //DACON2
          if(getSelectedMode(DACON2_PORTNUM) < NUMOFDACONDEVS-1)
          {
            uint8_t mode = getSelectedMode(DACON2_PORTNUM);
            setSelectedMode(DACON2_PORTNUM, ++mode);
          }
          else{
            setSelectedMode(DACON2_PORTNUM, DACONSTATEOFF);
          }
        }
        if(currentScreen == 6)
        { //Bounding Box Modes
          if(boundingboxmode < NUMOFBOUNDINGBOXMODES-1)
          {
            boundingboxmode++; 
          }
          else{
            boundingboxmode = 0;
          }
        }
        if(currentScreen == 7)
        {

          if(boundingboxsize < 25) boundingboxsize++;
        }
      }

      if(button & BUTTON_LEFT)
      {
        if(currentScreen == 0)
        { //DCON1
          if(getSelectedMode(DCON1_PORTNUM) > 0)
          {
            uint8_t mode = getSelectedMode(DCON1_PORTNUM);
            setSelectedMode(DCON1_PORTNUM, --mode);
          }
          else
          {
            setSelectedMode(DCON1_PORTNUM, NUMOFDCONDEVS-1);
          }
        }
        if(currentScreen == 1) 
        { //ACON1
          if(getSelectedMode(ACON1_PORTNUM) > 0)
          {
            uint8_t mode = getSelectedMode(ACON1_PORTNUM);
            setSelectedMode(ACON1_PORTNUM, --mode);
          }
          else
          {
            setSelectedMode(ACON1_PORTNUM, NUMOFACONDEVS-1);
          }
        }
        if(currentScreen == 2) 
        { //DACON1
          if(getSelectedMode(DACON1_PORTNUM) > 0)
          {
            uint8_t mode = getSelectedMode(DACON1_PORTNUM);
            setSelectedMode(DACON1_PORTNUM, --mode);
          }
          else
          {
            setSelectedMode(DACON1_PORTNUM, NUMOFDACONDEVS-1);
          }
        }
        if(currentScreen == 3) 
        { //DCON2
          if(getSelectedMode(DCON2_PORTNUM) > 0)
          {
            uint8_t mode = getSelectedMode(DCON2_PORTNUM);
            setSelectedMode(DCON2_PORTNUM, --mode);
          }
          else
          {
            setSelectedMode(DCON2_PORTNUM, NUMOFDCONDEVS-1);
          }
        }
        if(currentScreen == 4) 
        { //ACON2
          if(getSelectedMode(ACON2_PORTNUM) > 0)
          {
            uint8_t mode = getSelectedMode(ACON2_PORTNUM);
            setSelectedMode(ACON2_PORTNUM, --mode);
          }
          else
          {
            setSelectedMode(ACON2_PORTNUM, NUMOFACONDEVS-1);
          }
        }
        if(currentScreen == 5) 
        { //DACON2
          if(getSelectedMode(DACON2_PORTNUM) > 0)
          {
            uint8_t mode = getSelectedMode(DACON2_PORTNUM);
            setSelectedMode(DACON2_PORTNUM, --mode);
          }
          else
          {
            setSelectedMode(DACON2_PORTNUM, NUMOFDACONDEVS-1);
          }
        }
        if(currentScreen == 6) 
        { //bounding bnox mode screen
          if(boundingboxmode > 0)
          {
            boundingboxmode--;
          }
          else
          {
            boundingboxmode = NUMOFBOUNDINGBOXMODES-1;
          }
        }
        if(currentScreen == 7)
        {
          if(boundingboxsize > 3) boundingboxsize--;
        }
      }
    }

    //handes printing the menu, depending on what screen is active. 
    //because I have stored the strings in program memory (PROGMEM) we have to use special functions to retreive them
    //from the flash and load them into the stack as needed. This saves precious memory which we can use for other things like 
    //more affectors. Being as we are using dynamic object creation, the heap can become fragmented, so it is very important 
    //that we save as much space as possible to avoid heap collisions which will lead to a system crash. 
    // check out https://learn.adafruit.com/memories-of-an-arduino/optimizing-sram for more information. 
    void printMenu(_lcd* lcd) 
    {
      //the screen is 16 characters lone, but we need room for the null byte at the end of a character array
      char buffer[17]; 
      (lcd->lcd_obj)->clear();
      (lcd->lcd_obj)->setCursor(0,0);
      strcpy_P(buffer, (char *)pgm_read_word(&(screen1table[currentScreen])));
      (lcd->lcd_obj)->print(buffer);
      (lcd->lcd_obj)->setCursor(0,1);
      strcpy_P(buffer, (char *)pgm_read_word(&(screen2table[currentScreen])));
      (lcd->lcd_obj)->print(buffer);

      if(currentScreen == 0)
      { //DCON1 SCREEN
      strcpy_P(buffer, (char *)pgm_read_word(&(dconscreentable[getSelectedMode(DCON1_PORTNUM)])));
      (lcd->lcd_obj)->print(buffer);
      if(getSelectedMode(DCON1_PORTNUM) != getActiveMode(DCON1_PORTNUM)) (lcd->lcd_obj)->setBacklight(TEAL);
      else (lcd->lcd_obj)->setBacklight(WHITE);
      }
      if(currentScreen == 1)
      { //ACON1 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(aconscreentable[getSelectedMode(ACON1_PORTNUM)])));
        (lcd->lcd_obj)->print(buffer);
        if(getSelectedMode(ACON1_PORTNUM) != getActiveMode(ACON1_PORTNUM)) (lcd->lcd_obj)->setBacklight(TEAL);
        else (lcd->lcd_obj)->setBacklight(WHITE);
      }
      if(currentScreen == 2)
      { //DACON1 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(daconscreentable[getSelectedMode(DACON1_PORTNUM)])));
        (lcd->lcd_obj)->print(buffer);
        if(getSelectedMode(DACON1_PORTNUM) != getActiveMode(DACON1_PORTNUM)) (lcd->lcd_obj)->setBacklight(TEAL);
        else (lcd->lcd_obj)->setBacklight(WHITE);
      }
      if(currentScreen == 3)
      { //DCON2 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(dconscreentable[getSelectedMode(DCON2_PORTNUM)])));
        (lcd->lcd_obj)->print(buffer);
        if(getSelectedMode(DCON2_PORTNUM) != getActiveMode(DCON2_PORTNUM)) (lcd->lcd_obj)->setBacklight(TEAL);
        else (lcd->lcd_obj)->setBacklight(WHITE);
      }
      if(currentScreen == 4)
      { //ACON2 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(aconscreentable[getSelectedMode(ACON2_PORTNUM)])));
        (lcd->lcd_obj)->print(buffer);
        if(getSelectedMode(ACON2_PORTNUM) != getActiveMode(ACON2_PORTNUM)) (lcd->lcd_obj)->setBacklight(TEAL);
        else (lcd->lcd_obj)->setBacklight(WHITE);
      }
      if(currentScreen == 5)
      { //DACON2 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(daconscreentable[getSelectedMode(DACON2_PORTNUM)])));
        (lcd->lcd_obj)->print(buffer);
        if(getSelectedMode(DACON2_PORTNUM) != getActiveMode(DACON2_PORTNUM)) (lcd->lcd_obj)->setBacklight(TEAL);
        else (lcd->lcd_obj)->setBacklight(WHITE);
      }
      if(currentScreen == 6)
      {//BOUJND BOX MODE
        strcpy_P(buffer, (char *)pgm_read_word(&(boundingBoxModeScreensTable[boundingboxmode])));
        (lcd->lcd_obj)->print(buffer);
      }
      if(currentScreen == 7)
      { //BOUND BOX SIZE
        (lcd->lcd_obj)->print(boundingboxsize);
      }
      if(currentScreen == 8)
      { //VERSION
        (lcd->lcd_obj)->print(VERSIONNUMBER);
      }
    }
    //issues a warning to the LCD that too many affectors are active. 
    void issueWarning(_lcd* lcd_ptr, uint8_t type)
    {
      if(type == WARNINGTYPE_NUMDEVS)
      {
        (lcd_ptr->lcd_obj)->setBacklight(RED);
        (lcd_ptr->lcd_obj)->clear();
        (lcd_ptr->lcd_obj)->setCursor(0,0);
        (lcd_ptr->lcd_obj)->print(F("TOO MANY ACTIVE"));
        (lcd_ptr->lcd_obj)->setCursor(0,1);
        (lcd_ptr->lcd_obj)->print(F("AFFECTORS"));
        delay(2000); 
      }
      if(type == WARNINGTYPE_GROUP)
      {
        (lcd_ptr->lcd_obj)->setBacklight(RED);
        (lcd_ptr->lcd_obj)->clear();
        (lcd_ptr->lcd_obj)->setCursor(0,0);
        (lcd_ptr->lcd_obj)->print(F("ONLY ONE PORT"));
        (lcd_ptr->lcd_obj)->setCursor(0,1);
        (lcd_ptr->lcd_obj)->print(F("PER GROUP MAY"));
        delay(2000);
        (lcd_ptr->lcd_obj)->clear();
        (lcd_ptr->lcd_obj)->setCursor(0,0);
        (lcd_ptr->lcd_obj)->print(F("BE ACTIVE"));
        (lcd_ptr->lcd_obj)->setCursor(0,1);
        (lcd_ptr->lcd_obj)->print(F("SIMULTANIOUSLY"));
        delay(2000);
      }
    }
    ~_menu(){}
};

#endif