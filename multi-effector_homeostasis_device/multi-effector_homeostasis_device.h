#ifndef _MULTIHOMEO
#define _MULTIHOMEO

enum GAMESTATUS {
  notstarted,
  started,
  lost
};

enum SYSTEMMODE {
  NONE, //this is the default mode, but "default" is reserved by the compiler. 
  HANDGRIP,
  HANDCRANK,
  AuDIOVOLUME,
  AUDIOPITCH,
  ANOMETER
};

extern enum GAMESTATUS gameStatus;
extern enum SYSTEMMODE systemMode;

#define HANDGRIPPIN A0
#define ENCODERPINA 2
#define ENCODERPINB 3

#define HANDGRIPDEVNUM 0 //device number for handgrip
#define CRANKDEVNUM 1

#define HANDGRIPACTIVE 1
#define CRANKACTIVE 1

#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

// These #defines make it easy to set the backlight color for the RGB LCD Display
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define LCDREFRESHDELAY 500 //in ms

void navigateMenu(uint8_t button);

#endif