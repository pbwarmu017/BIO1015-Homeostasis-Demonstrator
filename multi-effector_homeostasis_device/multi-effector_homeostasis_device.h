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

#define STRIPTESTMODE 1 //used for internal testing

#endif