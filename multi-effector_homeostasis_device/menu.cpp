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
    int dcon1prevmode = -1
    int dcon2prevmode = -1
    int acon1prevmode = -1
    int acon2prevmode = -1
    int dacon1prevmode = -1
    int dacon2prevmode = -1
    int dcon1mode = 0;
    int dcon2mode = 0;
    int acon1mode = 0;
    int acon2mode = 0;
    int daconvmode = 0;
    int daconvmode = 0;
    
    
    

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
          if(acon1mode < numOfDconDevs-1){
          acon1prevmode = acon1mode;
          acon1mode++; 
          }
          else{
            acon1prevmode = acon1mode;
            acon1mode = 0;

          }
          if(acon1mode == 1 && dacon1mode == 1){ //only one member of group 1 can be active at one time. 
            dacon1mode = 0;
            dacon1prevmode = -1;
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
      }
    }
    void printMenu(_lcd* lcd) {
      (lcd->lcd_obj)->clear();
      (lcd->lcd_obj)->setCursor(0,0);
      (lcd->lcd_obj)->print(screens[currentScreen][0]);
      (lcd->lcd_obj)->setCursor(0,1);
      (lcd->lcd_obj)->print(screens[currentScreen][1]);
      (lcd->lcd_obj)->print(" ");
      if(currentScreen == 0){ //squeeze affector mode screen
        (lcd->lcd_obj)->print(modescreens[sqafmode]);
      }
      if(currentScreen == 1){ //squeeze affector port screen
        if(sqafmode == 1){
          (lcd->lcd_obj)->print(portscreens[sqafport]);
        }
        else{
          (lcd->lcd_obj)->print("N/A");
        }
      }
      if(currentScreen == 2){ //crank affector mode screen
        (lcd->lcd_obj)->print(modescreens[crafmode]);
      }
      if(currentScreen == 3){ //crank affector port screen
        if(crafmode == 1){
          (lcd->lcd_obj)->print(portscreens[crafport]);
        }
        else{
          (lcd->lcd_obj)->print("N/A");
        }
      }
    }
};

#endif