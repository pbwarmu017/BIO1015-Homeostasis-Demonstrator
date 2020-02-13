#ifndef MENU_CPP
#define MENU_CPP

#include "superclasses.cpp"
#include "lcd.cpp"
#include "multi-effector_homeostasis_device.h"

#define MAXIMUMDEVICES 2 //defines the max number of devices the system will allow to connect. 

//DEVICE STATES (used to track port status. As you add affectors and use these states, add descriptions here)
//note: there can only be 16 port states, as I am using only 4 bits to track it. 
#define DCONSTATE0 0 //OFF
#define DCONSTATE1 1 //HANDCRANK
#define DCONSTATE2 2
#define DCONSTATE3 3
#define DCONSTATE4 4
#define DCONSTATE5 5
#define DCONSTATE6 6
#define DCONSTATE7 7
#define DCONSTATE8 8
#define DCONSTATE9 9
#define DCONSTATE10 10
#define DCONSTATE11 11
#define DCONSTATE12 12
#define DCONSTATE13 13
#define DCONSTATE14 14
#define DCONSTATE15 15 //uninitialized


#define ACONSTATE0 0 //OFF
#define ACONSTATE1 1 //HANDGRIP
#define ACONSTATE2 2
#define ACONSTATE3 3
#define ACONSTATE4 4
#define ACONSTATE5 5
#define ACONSTATE6 6
#define ACONSTATE7 7
#define ACONSTATE8 8
#define ACONSTATE9 9
#define ACONSTATE10 10
#define ACONSTATE11 11
#define ACONSTATE12 12
#define ACONSTATE13 13
#define ACONSTATE14 14
#define ACONSTATE15 15 //uninitialized

#define DACONSTATE0 0 //OFF
#define DACONSTATE1 1
#define DACONSTATE2 2
#define DACONSTATE3 3
#define DACONSTATE4 4
#define DACONSTATE5 5
#define DACONSTATE6 6
#define DACONSTATE7 7
#define DACONSTATE8 8
#define DACONSTATE9 9
#define DACONSTATE10 10
#define DACONSTATE11 11
#define DACONSTATE12 12
#define DACONSTATE13 13
#define DACONSTATE14 14
#define DACONSTATE15 15 //uninitialized 

#define BOUNDINGBOXSTATIONARY 0
#define BOUNDINGBOXMOVING 1

#define DCON1FLAG 0
#define DCON2FLAG 1
#define ACON1FLAG 2
#define ACON2FLAG 3
#define DACON1FLAG 4
#define DACON2FLAG 5

//this is some PROGMEM magic to move the strings to program memory and free up SRAM
//LCD MENU TEXT OPTIONS

//Main Menu Screens (line 1)
#define NUMOFSCREENS 9 //number of options in screens[]
constexpr const char screen_1_1[17] PROGMEM = "PORT DCON1";
constexpr const char screen_2_1[17] PROGMEM = "PORT ACON1";
constexpr const char screen_3_1[17] PROGMEM = "PORT DACON1";
constexpr const char screen_4_1[17] PROGMEM = "PORT DCON2";
constexpr const char screen_5_1[17] PROGMEM = "PORT ACON2";
constexpr const char screen_6_1[17] PROGMEM = "PORT DACON2";
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
constexpr const char aconscreen_2[17] PROGMEM = {"SQUEEZE GRIP"};
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
    uint8_t objectCount = 0;
    int currentScreen = 0;
    //The following are selection tracking variables. As the user uses the
    //menu the contents of these values will change.
    int8_t dcon1prevmode = -1;
    int8_t dcon2prevmode = -1;
    int8_t acon1prevmode = -1;
    int8_t acon2prevmode = -1;
    int8_t dacon1prevmode = -1;
    int8_t dacon2prevmode = -1;
    int8_t dcon1mode = 0;
    int8_t dcon2mode = 0;
    int8_t acon1mode = 0;
    int8_t acon2mode = 0;
    int8_t dacon1mode = 0;
    int8_t dacon2mode = 0;
    //each of these is being split into two 4-bit numbers for tracking status info to save SRAM. 
    //This means that you can have up to 16 different states per each port. 
    uint8_t dconmode = 0;
    uint8_t dconprevmode = 0b11111111;

    uint8_t aconmode = 0;
    uint8_t aconprevmode = 0b11111111;

    uint8_t daconmode = 0;
    uint8_t daconprevmode = 0b11111111;

    int8_t boundingboxmode = 0;
    uint8_t boundingboxsize = 10;

    // int8_t prevMode = 0;
    // uint8_t currentMode = 0;

    void setPrevMode(const uint8_t flag, uint8_t val)
    {
      switch(flag)
      {
        case DCON1FLAG:
        dconprevmode &= 0b00001111; //clear the first four bits
        dconprevmode |= (val << 4); //set the first four bits to val
        break;

        case DCON2FLAG:
        dconprevmode &= 0b11110000; //clear the last four bits
        dconprevmode |= (val); //set the last four bits to val
        break;
        case ACON1FLAG:
        aconprevmode &= 0b00001111; //clear the first four bits
        aconprevmode |= (val << 4); //set the first four bits to val
        break;

        case ACON2FLAG:
        aconprevmode &= 0b11110000; //clear the first four bits
        aconprevmode |= (val); //set the first four bits to val
        break;
        case DACON1FLAG:
        daconprevmode &= 0b00001111; //clear the first four bits
        daconprevmode |= (val << 4);//set the first four bits to val
        break;

        case DACON2FLAG:
        daconprevmode &= 0b11110000; //clear the first four bits
        daconprevmode |= (val); //set the first four bits to val
        break;
      }
    }

    uint8_t getMode(const uint8_t flag)
    {
      switch(flag)
      {
        case DCON1FLAG:
        return((dconmode >> 4) & 0b00001111);
        break;

        case DCON2FLAG:
        return((dconmode) & 0b00001111);
        break;

        case ACON1FLAG:
        return((aconmode >> 4) & 0b00001111);
        break;

        case ACON2FLAG:
        return((aconmode) & 0b00001111);
        break;

        case DACON1FLAG:
        return((daconmode >> 4) & 0b00001111);
        break;

        case DACON2FLAG:
        return((daconmode) & 0b00001111);
        break;
      }
    }

    uint8_t getPrevMode(const uint8_t flag)
    {
      switch(flag)
      {
        case DCON1FLAG:
        return((dconprevmode >> 4) & 0b00001111);
        break;

        case DCON2FLAG:
        return((dconprevmode) & 0b00001111);
        break;

        case ACON1FLAG:
        return((aconprevmode >> 4) & 0b00001111);
        break;

        case ACON2FLAG:
        return((aconprevmode) & 0b00001111);
        break;

        case DACON1FLAG:
        return((daconprevmode >> 4) & 0b00001111);
        break;

        case DACON2FLAG:
        return((daconprevmode) & 0b00001111);
        break;
      }
    }

    void setMode(const uint8_t flag, uint8_t val)
    {
      switch(flag)
      {
        case DCON1FLAG:
        dconmode &= 0b00001111; //clear the first four bits
        dconmode |= (val << 4); //set the first four bits to val
        break;

        case DCON2FLAG:
        dconmode &= 0b11110000; //clear the last four bits
        dconmode |= (val); //set the last four bits to val
        break;
        case ACON1FLAG:
        aconmode &= 0b00001111; //clear the first four bits
        aconmode |= (val << 4); //set the first four bits to val
        break;

        case ACON2FLAG:
        aconmode &= 0b11110000; //clear the first four bits
        aconmode |= (val); //set the first four bits to val
        break;
        case DACON1FLAG:
        daconmode &= 0b00001111; //clear the first four bits
        daconmode |= (val << 4);//set the first four bits to val
        break;

        case DACON2FLAG:
        daconmode &= 0b11110000; //clear the first four bits
        daconmode |= (val); //set the first four bits to val
        break;
      }
    }
    
    void navigateMenu(int button, _lcd* lcd_ptr)
    {
      if(button & BUTTON_UP) 
      {
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
      if(button & BUTTON_LEFT) 
      {
        parameterChange(0, lcd_ptr);
        printMenu(lcd_ptr);
        //the created objects might need control of the LCD. The created objects will print the menu when they are finished 
        //with the LCD
        //if the parameter change does not create or destroy  an object, print the menu in parameterchange(). 
      }
      if(button & BUTTON_RIGHT) 
      {
        parameterChange(1, lcd_ptr); 
        printMenu(lcd_ptr);

        //the created objects might need control of the LCD. The objects will print the menu when they are finished 
        //with the LCD 
        //if the parameter change does not create or destroy  an object, print the menu in parameterchange().
      }
      if(button & BUTTON_SELECT)
      {
        applyParameter(lcd_ptr);
      }
    }

    void applyParameter(_lcd * lcd_ptr)
    {
      if(currentScreen == 0) 
      {//DCON1
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          dcon1mode = 0; //reset the menu selection
          printMenu(lcd_ptr);
          if(dcon1prevmode == DCONSTATE1)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON1_PORTNUM);
          }
        }
        else
        {
          if(dcon1prevmode == DCONSTATE1)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON1_PORTNUM);
          }
          if(dcon1mode == DCONSTATE1)
          {
            createObject(HANDCRANK_TYPE, DCON1_PORTNUM);
          } 
        }
      }
      if(currentScreen == 1)
      { //ACON1
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          acon1mode = 0; //reset the menu selection
          printMenu(lcd_ptr);
          if(acon1prevmode == ACONSTATE1)
          { //if there has been a handgrip configured previously
            deleteObject(HANDGRIP_TYPE, ACON1_PORTNUM);
          }
        }
        else
        {
          // if(dacon1mode == ACONSTATE1)
          // { //only one member of group 1 can be active at one time.
          //   deleteObject(HANDGRIP_TYPE, DACON1_PORTNUM); 
          //   dacon1prevmode = dacon1mode;
          //   dacon1mode = 0;
          //   issueDuplicationWarning(lcd_ptr);
          // }
          if(acon1prevmode == ACONSTATE1)
          { //if there has been a handgrip configured previously
            deleteObject(HANDGRIP_TYPE, ACON1_PORTNUM);
          }
          if(acon1mode == ACONSTATE1)
          {
            createObject(HANDGRIP_TYPE, ACON1_PORTNUM);
          }
        }
      }
      if(currentScreen == 2)
      { //DACON1
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
            dacon1mode = 0;
            printMenu(lcd_ptr);
        }
        else
        {
          // if(acon1mode == ACONSTATE1)
          // { //only one member of group 1 can be active at one time.
          //   deleteObject(HANDGRIP_TYPE, ACON1_PORTNUM); 
          //   acon1prevmode = acon1mode;
          //   acon1mode = 0;
          //   issueDuplicationWarning(lcd_ptr);
          // }
          // if(dacon1prevmode == DACONSTATE1)
          // { //if there has been a handgrip configured previously
          //   deleteObject(HANDGRIP_TYPE, DACON1_PORTNUM); 
          // }
          // if(dacon1mode == DACONSTATE1)
          // {
          //   createObject(HANDGRIP_TYPE, DACON1_PORTNUM);
          // }
        }
      } 
      if(currentScreen == 3) 
      {//DCON2
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          dcon2mode = 0;
          printMenu(lcd_ptr);
          if(dcon2prevmode == DCONSTATE1)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON2_PORTNUM);
          }
        }
        else
        {
          if(dcon2prevmode == DCONSTATE1)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON2_PORTNUM);
          }
          if(dcon2mode == DCONSTATE1)
          {
            createObject(HANDCRANK_TYPE, DCON2_PORTNUM);
          } 
        }
      }
      if(currentScreen == 4) 
      {//ACON2
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          acon2mode = 0; //reset the menu selection
          printMenu(lcd_ptr);
          if(acon2prevmode == ACONSTATE1)
          { //if there has been a handgrip configured previously
            deleteObject(HANDGRIP_TYPE, ACON2_PORTNUM);
          }
        }
        else
        {
          // if(dacon2mode == ACONSTATE1)
          // { //only one member of group 1 can be active at one time.
          //   deleteObject(HANDGRIP_TYPE, DACON2_PORTNUM); 
          //   dacon1prevmode = dacon2mode;
          //   dacon2mode = 0;
          //   issueDuplicationWarning(lcd_ptr);
          // }
          if(acon2prevmode == ACONSTATE1)
          { //if there has been a handgrip configured previously
            deleteObject(HANDGRIP_TYPE, ACON2_PORTNUM);
          }
          if(acon2mode == ACONSTATE1)
          {
            createObject(HANDGRIP_TYPE, ACON2_PORTNUM);
          }
        }
      }
      if(currentScreen == 5)
      {//DACON2
        if(objectCount == MAXIMUMDEVICES) //only allow so many devices
        {
          dacon2mode = 0; //reset the menu selection
          printMenu(lcd_ptr);
        }
        else
        {
          // if(acon2mode == ACONSTATE1)
          // { //only one member of group 1 can be active at one time.
          //   deleteObject(HANDGRIP_TYPE, ACON2_PORTNUM); 
          //   acon2prevmode = acon2mode;
          //   acon2mode = 0;
          //   issueDuplicationWarning(lcd_ptr);
          // }
          // if(dacon2prevmode == DACONSTATE1)
          // { //if there has been a handgrip configured previously
          //   deleteObject(HANDGRIP_TYPE, DACON2_PORTNUM); 
          // }
          // if(dacon2mode == DACONSTATE1)
          // {
          //   createObject(HANDGRIP_TYPE, DACON2_PORTNUM);
          // }
        }
      }
      (lcd_ptr->lcd_obj)->setBacklight(WHITE);
    }

    void parameterChange(int index, _lcd* lcd_ptr)
    {
      if(index == 1)
      {
        if(currentScreen == 0)
        { //DCON1
          if(dcon1mode < NUMOFDCONDEVS-1)
          {
            dcon1prevmode = dcon1mode;
            dcon1mode++; 
          }
          else
          {
            dcon1prevmode = dcon1mode;
            dcon1mode = 0;
          }
        }
        if(currentScreen == 1)
        { //ACON1
          if(acon1mode < NUMOFACONDEVS-1)
          {
            acon1prevmode = acon1mode;
            acon1mode++; 
          }
          else
          {
            acon1prevmode = acon1mode;
            acon1mode = 0;
          }
        }
        if(currentScreen == 2)
        { //DACON1
          if(dacon1mode < NUMOFACONDEVS-1)
          {
            dacon1prevmode = dacon1mode;
            dacon1mode++; 
          }
          else
          {
            dacon1prevmode = dacon1mode;
            dacon1mode = 0;
          }
        }
        if(currentScreen == 3)
        { //DCON2
          if(dcon2mode < NUMOFDCONDEVS-1)
          {
          dcon2prevmode = dcon2mode;
          dcon2mode++; 
          }
          else
          {
            dcon2prevmode = dcon2mode;
            dcon2mode = 0;
          } 
        }
        if(currentScreen == 4)
        { //ACON2
          
          if(acon2mode < NUMOFACONDEVS-1)
          {
            acon2prevmode = acon2mode;
            acon2mode++; 
          }
          else
          {
            acon2prevmode = acon2mode;
            acon2mode = 0;
          }
        }
        if(currentScreen == 5)
        { //DACON2
          if(dacon2mode < NUMOFACONDEVS-1)
          {
            dacon2prevmode = dacon2mode;
            dacon2mode++; 
          }
          else{
            dacon2prevmode = dacon2mode;
            dacon2mode = 0;
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
        if(currentScreen != 6 && currentScreen != 7) (lcd_ptr->lcd_obj)->setBacklight(TEAL);
      }

      if(index == 0)
      {
        if(currentScreen == 0)
        { //DCON1
          if(dcon1mode > 0)
          {
            dcon1prevmode = dcon1mode;
            dcon1mode--;
          }
          else
          {
            dcon1prevmode = dcon1mode;
            dcon1mode = NUMOFDCONDEVS-1;
          }
        }
        if(currentScreen == 1) 
        { //ACON1
          if(acon1mode > 0)
          {
            acon1prevmode = acon1mode;
            acon1mode--;
          }
          else
          {
            acon1prevmode = acon1mode;
            acon1mode = NUMOFDCONDEVS-1;
          }
        }
        if(currentScreen == 2) 
        { //DACON1
          if(dacon1mode > 0)
          {
            dacon1prevmode = dacon1mode;
            dacon1mode--;
          }
          else
          {
            dacon1prevmode = dacon1mode;
            dacon1mode = NUMOFDCONDEVS-1;
          }
        }
        if(currentScreen == 3) 
        { //DCON2
          if(dcon2mode > 0)
          {
            dcon2prevmode = dcon2mode;
            dcon2mode--;
          }
          else
          {
            dcon2prevmode = dcon2mode;
            dcon2mode = NUMOFDCONDEVS-1;
          }
        }
        if(currentScreen == 4) 
        { //ACON2
          if(acon2mode > 0)
          {
            acon2prevmode = acon2mode;
            acon2mode--;
          }
          else
          {
            acon2prevmode = acon2mode;
            acon2mode = NUMOFDCONDEVS-1;
          }
        }
        if(currentScreen == 5) 
        { //DACON2
          if(dacon2mode > 0)
          {
            dacon2prevmode = dacon2mode;
            dacon2mode--;
          }
          else
          {
            dacon2prevmode = dacon2mode;
            dacon2mode = NUMOFDCONDEVS-1;
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
        if(currentScreen != 6 && currentScreen != 7) (lcd_ptr->lcd_obj)->setBacklight(TEAL);
      }
    }
    void printMenu(_lcd* lcd) 
    {
      char buffer[17]; 
      (lcd->lcd_obj)->clear();
      (lcd->lcd_obj)->setCursor(0,0);
      // char test = *screen1table[0];
      strcpy_P(buffer, (char *)pgm_read_word(&(screen1table[currentScreen])));
      (lcd->lcd_obj)->print(buffer);
      (lcd->lcd_obj)->setCursor(0,1);
      strcpy_P(buffer, (char *)pgm_read_word(&(screen2table[currentScreen])));
      (lcd->lcd_obj)->print(buffer);
      // strcpy_P(buffer, (char *)pgm_read_word(&(screen2table[1])));
      // (lcd->lcd_obj)->print(screens[currentScreen][1]);

      // (lcd->lcd_obj)->print(" ");
      if(currentScreen == 0)
      { //DCON1 SCREEN
      strcpy_P(buffer, (char *)pgm_read_word(&(dconscreentable[dcon1mode])));
      (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(dconscreens[dcon1mode]);
      }
      if(currentScreen == 1)
      { //ACON1 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(aconscreentable[acon1mode])));
        (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(aconscreens[acon1mode]);
      }
      if(currentScreen == 2)
      { //DACON1 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(daconscreentable[dacon1mode])));
        (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(daconscreens[dacon1mode]);
      }
      if(currentScreen == 3)
      { //DCON2 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(dconscreentable[dcon2mode])));
        (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(dconscreens[dcon2mode]);
      }
      if(currentScreen == 4)
      { //ACON2 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(aconscreentable[acon2mode])));
        (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(aconscreens[acon2mode]);
      }
      if(currentScreen == 5)
      { //DACON2 SCREEN
        strcpy_P(buffer, (char *)pgm_read_word(&(daconscreentable[dacon2mode])));
        (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(daconscreens[dacon2mode]);
      }
      if(currentScreen == 6)
      {//BOUJND BOX MODE
        strcpy_P(buffer, (char *)pgm_read_word(&(boundingBoxModeScreensTable[boundingboxmode])));
        (lcd->lcd_obj)->print(buffer);
        // (lcd->lcd_obj)->print(boundingBoxModeScreens[boundingboxmode]);
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
    // void issueDuplicationWarning(_lcd* lcd_ptr)
    // {
    //   (lcd_ptr->lcd_obj)->clear();
    //   (lcd_ptr->lcd_obj)->setCursor(0,0);
    //   (lcd_ptr->lcd_obj)->print("One 1 Member Of");
    //   (lcd_ptr->lcd_obj)->setCursor(0,1);
    //   (lcd_ptr->lcd_obj)->print("A Group Can Be");
    //   delay(2000);
    //   (lcd_ptr->lcd_obj)->clear();
    //   (lcd_ptr->lcd_obj)->setCursor(0,0);
    //   (lcd_ptr->lcd_obj)->print("Active At The");
    //   (lcd_ptr->lcd_obj)->setCursor(0,1);
    //   (lcd_ptr->lcd_obj)->print("Same time");
    //   delay(2000);
    //   printMenu(lcd_ptr);
    // }
    ~_menu()
    {
      //must define a destructor, but I have no use for it here. 
    }
};

#endif