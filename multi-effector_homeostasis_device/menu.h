#ifndef _MENUH
#define _MENUH


class _menu {
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
  
  int navigateMenu(int, Adafruit_RGBLCDShield*);
  void parameterChange(int);
  void printMenu(Adafruit_RGBLCDShield*);

};

//function declarations

#endif