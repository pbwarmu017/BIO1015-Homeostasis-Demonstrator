#include "headers.h"

const int numOfScreens = 4;  //number of options in screens[]
const int numOfModes = 2; //number of options in modescreens[]
const int numOfPorts = 6; //number of options in portscreens[]

//The following are selection tracking variables. As the user uses the
//menu the contents of these variables will change.
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
      if(sqafmode < numOfModes) sqafmode++;
      else sqafmode = 1;
    }
    if(currentScreen == 1) {
      if(sqafport < numOfPorts) sqafport++;
      else sqafport = 1;
    }
    if(currentScreen == 2) {
      if(crafmode < numOfModes) crafmode++;
      else crafmode = 1;
    }
    if(currentScreen == 3) {
      if(crafport < numOfPorts) crafport++;
      else crafport = 1;
    }
  }
  if(index == 0){
    if(currentScreen == 0) {
      if(sqafmode > 0) sqafmode--;
      else sqafmode = numOfModes;
    }
    if(currentScreen == 1) {
      if(sqafport > 0) sqafport--;
      else sqafport = numOfPorts;
    } 
    if(currentScreen == 2) {
      if(crafmode > 0) crafmode--;
      else crafmode = numOfModes;
    }
    if(currentScreen == 3) {
      if(crafport > 0) crafport--;
      else crafport = numOfPorts;
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