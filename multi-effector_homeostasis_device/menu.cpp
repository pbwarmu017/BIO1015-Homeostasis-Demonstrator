#include "superclasses.cpp"

#ifndef MENU_CPP
#define MENU_CPP

class _menu: public _device{
  public:
    static const int numOfScreens = 7;  //number of options in screens[]
    static const int numOfDconDevs = 2; //number of options in dconscreens[]
    static const int numOfAconDevs = 2; //number of options in aconscreens[]
    static const int numOfDaconDevs = 2; //number of options in daconscreens[]
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
    String screens[numOfScreens][2] = {{"DCON1","DEV:"},
      {"ACON1","DEV:"},{"DACON1","DEV:"},{"DCON2","DEV:"},{"ACON2","DEV:"},{"DACON2","DEV:"},
      {"PORT GROUPS","SHARE ANALOG CH!"}};

    //Valid selections for each port type
    String dconscreens[2] = {"OFF","CRANK"};
    String aconscreens[2] = {"OFF","SQUEEZE"};
    String daconscreens[2] = {"OFF","SQUEEZE"};

    //Port Selections. If you add to this, increase numOfPorts to match. 
    // String portscreens[6] = {"DCON1","ACON1","DACON1","DCON2","ACON2","DACON2"};
    
    void navigateMenu(int button, _lcd* lcd, _device *DCON1_ptr, _device *ACON1_ptr, _device *DACON1_ptr, 
                    _device *DCON2_ptr, _device *ACON2_ptr, _device *DACON2_ptr){
      if(button & BUTTON_UP) {
        if (currentScreen == 0) {
              currentScreen = numOfScreens-2;
              printMenu(lcd);
            }
            else{
              currentScreen--;
              printMenu(lcd);
            }
      }
      if(button & BUTTON_DOWN){
        if (currentScreen >= numOfScreens-2) {
              currentScreen = 0;
              printMenu(lcd);
            }
        else{
          currentScreen++;
          printMenu(lcd);
        }
      }
      if(button & BUTTON_LEFT) {
        parameterChange(0, DCON1_ptr, ACON1_ptr, DACON1_ptr, DCON2_ptr, ACON2_ptr, DACON2_ptr);
        printMenu(lcd);
      }
      if(button & BUTTON_RIGHT) {
        parameterChange(1, DCON1_ptr, ACON1_ptr, DACON1_ptr, DCON2_ptr, ACON2_ptr, DACON2_ptr);
        printMenu(lcd);
      }
    }

    void parameterChange(int index, _device *DCON1_ptr, _device *ACON1_ptr, _device *DACON1_ptr, _device *DCON2_ptr,
                      _device *ACON2_ptr, _device *DACON2_ptr) {
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
          if(acon1prevmode != -1) ~*ACON1_ptr();
          if(acon1mode < numOfAconDevs-1){
          acon1prevmode = acon1mode;
          acon1mode++; 
          if(acon1mode == 1) *ACON1_ptr = new _handgrip;
          }
          else{
            acon1prevmode = acon1mode;
            acon1mode = 0;
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