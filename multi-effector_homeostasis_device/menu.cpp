#ifndef MENU_CPP
#define MENU_CPP

#include "superclasses.cpp"
#include "lcd.cpp"
#include "multi-effector_homeostasis_device.h"

#define DCONHANDCRANK 1
#define ACONHANDGRIP 1
#define DACONHANDGRIP 1
#define BOUNDINGBOXSTATIONARY 0
#define BOUNDINGBOXMOVING 1
// #include "objects.h"

class _menu: public _device{
  public:
    static const byte numOfScreens = 8;  //number of options in screens[]
    static const byte numOfDconDevs = 2; //number of options in dconscreens[]
    static const byte numOfAconDevs = 2; //number of options in aconscreens[]
    static const byte numOfDaconDevs = 2; //number of options in daconscreens[]
    static const byte numOfBoundingBoxModes = 5; //number of options in boundingBoxModeScreens
    byte currentScreen = 0;
    //The following are selection tracking variables. As the user uses the
    //menu the contents of these values will change.
    byte dcon1prevmode = -1;
    byte dcon2prevmode = -1;
    byte acon1prevmode = -1;
    byte acon2prevmode = -1;
    byte dacon1prevmode = -1;
    byte dacon2prevmode = -1;
    byte dcon1mode = 0;
    byte dcon2mode = 0;
    byte acon1mode = 0;
    byte acon2mode = 0;
    byte dacon1mode = 0;
    byte dacon2mode = 0;
    byte boundingboxmode = 0;
    byte boundingboxsize = 10;
    
    
    

    //LCD MENU TEXT OPTIONS

    //Main Menu Screens
    const String screens[numOfScreens][2] = {{"DCON1","DEV:"},{"ACON1","DEV:"},{"DACON1","DEV:"},
    {"DCON2","DEV:"},{"ACON2","DEV:"},{"DACON2","DEV:"},{"BOUND","MODE:"},{"TEST","TESTING"}};

    //Valid selections for each port type
    const String dconscreens[numOfDconDevs] = {"OFF","CRANK"};
    const String aconscreens[numOfAconDevs] = {"OFF","GRIP"};
    const String daconscreens[numOfDaconDevs] = {"NONE AVAIL."};
    const String boundingBoxModeScreens[numOfBoundingBoxModes] = {{"Stat."},{"Mov 1"},{"Mov 2"},{"Mov 3"},{"Mov 4"}};

    //Port Selections. If you add to this, increase numOfPorts to match. 
    // String portscreens[6] = {"DCON1","ACON1","DACON1","DCON2","ACON2","DACON2"};
    
    void navigateMenu(int button, _lcd* lcd_ptr)
    {
      if(button & BUTTON_UP) 
      {
        if (currentScreen == 0) 
        {
          currentScreen = numOfScreens-1;
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
        if (currentScreen >= numOfScreens-1) 
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
        if(dcon1prevmode == DCONHANDCRANK)
          { //if there has been an encoder configured previously
            deleteObject(HANDCRANK_TYPE, DCON1_PORTNUM);
          }
        if(dcon1mode == DCONHANDCRANK)
        {
          createObject(HANDCRANK_TYPE, DCON1_PORTNUM);
        } 
      }
      if(currentScreen == 1)
      { //ACON1
        // if(dacon1mode == ACONHANDGRIP)
        // { //only one member of group 1 can be active at one time.
        //   deleteObject(HANDGRIP_TYPE, DACON1_PORTNUM); 
        //   dacon1prevmode = dacon1mode;
        //   dacon1mode = 0;
        //   issueDuplicationWarning(lcd_ptr);
        // }
        if(acon1prevmode == ACONHANDGRIP)
        { //if there has been a handgrip configured previously
          deleteObject(HANDGRIP_TYPE, ACON1_PORTNUM);
        }
        if(acon1mode == ACONHANDGRIP)
        {
          createObject(HANDGRIP_TYPE, ACON1_PORTNUM);
        }
      }
      if(currentScreen == 2)
      { //DACON1
        // if(acon1mode == ACONHANDGRIP)
        // { //only one member of group 1 can be active at one time.
        //   deleteObject(HANDGRIP_TYPE, ACON1_PORTNUM); 
        //   acon1prevmode = acon1mode;
        //   acon1mode = 0;
        //   issueDuplicationWarning(lcd_ptr);
        // }
        // if(dacon1prevmode == DACONHANDGRIP)
        // { //if there has been a handgrip configured previously
        //   deleteObject(HANDGRIP_TYPE, DACON1_PORTNUM); 
        // }
        // if(dacon1mode == DACONHANDGRIP)
        // {
        //   createObject(HANDGRIP_TYPE, DACON1_PORTNUM);
        // }
      }
      if(currentScreen == 3) 
      {//DCON2
        if(dcon2prevmode == DCONHANDCRANK)
        { //if there has been an encoder configured previously
          deleteObject(HANDCRANK_TYPE, DCON2_PORTNUM);
        }
        if(dcon2mode == DCONHANDCRANK)
        {
          createObject(HANDCRANK_TYPE, DCON2_PORTNUM);
        }
      }
      if(currentScreen == 4) 
      {//ACON2
        // if(dacon2mode == ACONHANDGRIP)
        // { //only one member of group 1 can be active at one time.
        //   deleteObject(HANDGRIP_TYPE, DACON2_PORTNUM); 
        //   dacon1prevmode = dacon2mode;
        //   dacon2mode = 0;
        //   issueDuplicationWarning(lcd_ptr);
        // }
        if(acon2prevmode == ACONHANDGRIP)
        { //if there has been a handgrip configured previously
          deleteObject(HANDGRIP_TYPE, ACON2_PORTNUM);
        }
        if(acon2mode == ACONHANDGRIP)
        {
          createObject(HANDGRIP_TYPE, ACON2_PORTNUM);
        }
      }
      if(currentScreen == 5)
      {//DACON2
        // if(acon2mode == ACONHANDGRIP)
        // { //only one member of group 1 can be active at one time.
        //   deleteObject(HANDGRIP_TYPE, ACON2_PORTNUM); 
        //   acon2prevmode = acon2mode;
        //   acon2mode = 0;
        //   issueDuplicationWarning(lcd_ptr);
        // }
        // if(dacon2prevmode == DACONHANDGRIP)
        // { //if there has been a handgrip configured previously
        //   deleteObject(HANDGRIP_TYPE, DACON2_PORTNUM); 
        // }
        // if(dacon2mode == DACONHANDGRIP)
        // {
        //   createObject(HANDGRIP_TYPE, DACON2_PORTNUM);
        // }
      }
      (lcd_ptr->lcd_obj)->setBacklight(WHITE);
    }

    void parameterChange(int index, _lcd* lcd_ptr)
    {
      if(index == 1)
      {
        if(currentScreen == 0)
        { //DCON1
          if(dcon1mode < numOfDconDevs-1)
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
          if(acon1mode < numOfAconDevs-1)
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
          if(dacon1mode < numOfAconDevs-1)
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
          if(dcon2mode < numOfDconDevs-1)
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
          
          if(acon2mode < numOfAconDevs-1)
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
          if(dacon2mode < numOfAconDevs-1)
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
          if(boundingboxmode < numOfBoundingBoxModes-1)
          {
            boundingboxmode++; 
          }
          else{
            boundingboxmode = 0;
          }
        }
        if(currentScreen != 6) (lcd_ptr->lcd_obj)->setBacklight(TEAL);
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
            dcon1mode = numOfDconDevs-1;
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
            acon1mode = numOfDconDevs-1;
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
            dacon1mode = numOfDconDevs-1;
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
            dcon2mode = numOfDconDevs-1;
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
            acon2mode = numOfDconDevs-1;
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
            dacon2mode = numOfDconDevs-1;
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
            boundingboxmode = numOfBoundingBoxModes-1;
          }
        }
        if(currentScreen != 6) (lcd_ptr->lcd_obj)->setBacklight(TEAL);
      }
    }
    void printMenu(_lcd* lcd) 
    {
      (lcd->lcd_obj)->clear();
      (lcd->lcd_obj)->setCursor(0,0);
      (lcd->lcd_obj)->print(screens[currentScreen][0]);
      (lcd->lcd_obj)->setCursor(0,1);
      (lcd->lcd_obj)->print(screens[currentScreen][1]);
      (lcd->lcd_obj)->print(" ");
      if(currentScreen == 0)
      { //DCON1 SCREEN
        (lcd->lcd_obj)->print(dconscreens[dcon1mode]);
      }
      if(currentScreen == 1)
      { //ACON1 SCREEN
        (lcd->lcd_obj)->print(aconscreens[acon1mode]);
      }
      if(currentScreen == 2)
      { //DACON1 SCREEN
        (lcd->lcd_obj)->print(daconscreens[dacon1mode]);
      }
      if(currentScreen == 3)
      { //DCON2 SCREEN
        (lcd->lcd_obj)->print(dconscreens[dcon2mode]);
      }
      if(currentScreen == 4)
      { //ACON2 SCREEN
        (lcd->lcd_obj)->print(aconscreens[acon2mode]);
      }
      if(currentScreen == 5)
      { //DACON2 SCREEN
        (lcd->lcd_obj)->print(daconscreens[dacon2mode]);
      }
      if(currentScreen == 6)
      {
        (lcd->lcd_obj)->print(boundingBoxModeScreens[boundingboxmode]);
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