#include "superclasses.cpp"
#include "lcd.cpp"

#ifndef MENU_CPP
#define MENU_CPP

class _menu: public _device{
  public:
    static const int numOfScreens = 6;  //number of options in screens[]
    static const int numOfDconDevs = 1; //number of options in dconscreens[]
    static const int numOfAconDevs = 1; //number of options in aconscreens[]
    static const int numOfDaconDevs = 1; //number of options in daconscreens[]
    int currentScreen = 0;
    //The following are selection tracking variables. As the user uses the
    //menu the contents of these values will change.
    int dcon1prevmode = -1;
    int dcon2prevmode = -1;
    int acon1prevmode = -1;
    int acon2prevmode = -1;
    int dacon1prevmode = -1;
    int dacon2prevmode = -1;
    int dcon1mode = 0;
    int dcon2mode = 0;
    int acon1mode = 0;
    int acon2mode = 0;
    int dacon1mode = 0;
    int dacon2mode = 0;
    
    
    

    //LCD MENU TEXT OPTIONS

    //Main Menu Screens
    String screens[numOfScreens][2] = {{"DCON1","DEV: "},
      {"DCON2","DEV: "},{"ACON1","DEV:"},{"ACON2","DEV: "},{"DACON1","DEV: "},{"DACON2","DEV: "}};

    //Valid selections for each port type
    String dconscreens[2] = {"OFF","CRANK"};
    String aconscreens[2] = {"OFF","SQUEEZE"};
    String daconscreens[2] = {"OFF","SQUEEZE"};

    //Port Selections. If you add to this, increase numOfPorts to match. 
    // String portscreens[6] = {"DCON1","ACON1","DACON1","DCON2","ACON2","DACON2"};
    
    void navigateMenu(int button, _lcd* lcd){
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
    }

    void parameterChange(int index) {
      if(index == 1){
        if(currentScreen == 0){ //DCON1
          if(dcon1mode < numOfDconDevs-1){
          dcon1prevmode = dcon1mode;
          dcon1mode++; 
          }
          else{
            dcon1prevmode = dcon1mode;
            dcon1mode = 0;
          } 
        }
        if(currentScreen == 1){ //ACON1
          if(acon1mode < numOfAconDevs-1){
          acon1prevmode = acon1mode;
          acon1mode++; 
          }
          else{
            acon1prevmode = acon1mode;
            acon1mode = 0;

          }
          if(acon1mode == 1 && dacon1mode == 1){ //only one member of group 1 can be active at one time. 
            dacon1prevmode = dacon1mode; //deactivate the other one
            dacon1mode = 0;
          }
        }
        if(currentScreen == 2){ //DACON1
          if(dacon1mode < numOfDaconDevs-1){
          dacon1prevmode = dacon1mode;
          dacon1mode++; 
          }
          else{
            dacon1prevmode = dacon1mode;
            dacon1mode = 0;

          }
          if(dacon1mode == 1 && acon1mode == 1){ //only one member of group 1 can be active at one time. 
            acon1prevmode = acon1mode; //deactivate the other one
            acon1mode = 0;
          }
        }
        if(currentScreen == 3){ //DCON2
          if(dcon2mode < numOfDconDevs-1){
          dcon2prevmode = dcon2mode;
          dcon2mode++; 
          }
          else{
            dcon2prevmode = dcon2mode;
            dcon2mode = 0;
          } 
        }
        if(currentScreen == 4){ //ACON2
          if(acon2mode < numOfAconDevs-1){
          acon2prevmode = acon2mode;
          acon2mode++; 
          }
          else{
            acon2prevmode = acon2mode;
            acon2mode = 0;

          }
          if(acon2mode == 1 && dacon2mode == 1){ //only one member of group 1 can be active at one time. 
            dacon2mode = 0;
            dacon2prevmode = -1;
          }
        }
        if(currentScreen == 5){ //DACON2
          if(dacon2mode < numOfDaconDevs-1){
          dacon2prevmode = dacon2mode;
          dacon2mode++; 
          }
          else{
            dacon2prevmode = dacon2mode;
            dacon2mode = 0;

          }
          if(dacon2mode == 1 && acon2mode == 1){ //only one member of group 1 can be active at one time. 
            acon2prevmode =acon2mode;
            acon2mode = 0;
          }
        }
      }

      if(index == 0){
        if(currentScreen == 0) { //DCON1
          if(dcon1mode > 0){
            dcon1prevmode = dcon1mode;
            dcon1mode--;
          }
          else{
            dcon1prevmode = dcon1mode;
            dcon1mode = numOfDconDevs-1;
          }
        }
        if(currentScreen == 1) { //ACON1
          if(acon1mode > 0){
            acon1prevmode = acon1mode;
            acon1mode--;
          }
          else{
            acon1prevmode = acon1mode;
            acon1mode = numOfDconDevs-1;
          }
          if(acon1mode == 1 && dacon1mode == 1){ //only one member of group 1 can be active at one time. 
            dacon1prevmode = dacon1mode; //deactivate the other one
            dacon1mode = 0;
          }
        }
        if(currentScreen == 2) { //DACON1
          if(dacon1mode > 0){
            dacon1prevmode = dacon1mode;
            dacon1mode--;
          }
          else{
            dacon1prevmode = dacon1mode;
            dacon1mode = numOfDconDevs-1;
          }
          if(dacon1mode == 1 && acon1mode == 1){ //only one member of group 1 can be active at one time. 
            acon1prevmode = acon1mode; //deactivate the other one
            acon1mode = 0;
          }
        }
        if(currentScreen == 3) { //DCON2
          if(dcon2mode > 0){
            dcon2prevmode = dcon2mode;
            dcon2mode--;
          }
          else{
            dcon2prevmode = dcon2mode;
            dcon2mode = numOfDconDevs-1;
          }
        }
        if(currentScreen == 4) { //ACON2
          if(acon2mode > 0){
            acon2prevmode = acon2mode;
            acon2mode--;
          }
          else{
            acon2prevmode = acon2mode;
            acon2mode = numOfDconDevs-1;
          }
          if(acon2mode == 1 && dacon2mode == 1){ //only one member of group 2 can be active at one time. 
            dacon2prevmode = dacon2mode; //deactivate the other one
            dacon2mode = 0;
          }
        }
        if(currentScreen == 5) { //DACON2
          if(dacon2mode > 0){
            dacon2prevmode = dacon2mode;
            dacon2mode--;
          }
          else{
            dacon2prevmode = dacon2mode;
            dacon2mode = numOfDconDevs-1;
          }
          if(dacon2mode == 1 && acon2mode == 1){ //only one member of group 2 can be active at one time. 
            acon2prevmode = acon2mode; //deactivate the other one
            acon2mode = 0;
          }
        }
      }
    }
    void printMenu(_lcd* lcd) {
      (lcd->lcd_obj)->clear();
      (lcd->lcd_obj)->setCursor(0,0);
      (lcd->lcd_obj)->print(screens[currentScreen][0]);
      (lcd->lcd_obj)->setCursor(0,1);
      (lcd->lcd_obj)->print(screens[currentScreen][1]);
      (lcd->lcd_obj)->print(" ");
      if(currentScreen == 0){ //DCON1 SCREEN
        (lcd->lcd_obj)->print(dconscreens[dcon1mode]);
      }
      if(currentScreen == 1){ //ACON1 SCREEN
        (lcd->lcd_obj)->print(aconscreens[acon1mode]);
      }
      if(currentScreen == 2){ //DACON1 SCREEN
        (lcd->lcd_obj)->print(daconscreens[dacon1mode]);
      }
      if(currentScreen == 3){ //DCON2 SCREEN
        (lcd->lcd_obj)->print(dconscreens[dcon2mode]);
      }
      if(currentScreen == 4){ //ACON2 SCREEN
        (lcd->lcd_obj)->print(aconscreens[acon2mode]);
      }
      if(currentScreen == 5){ //DACON2 SCREEN
        (lcd->lcd_obj)->print(daconscreens[dacon2mode]);
      }
    }
};

#endif