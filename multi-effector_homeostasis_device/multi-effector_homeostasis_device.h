#ifndef _MULTIHOMEO
#define _MULTIHOMEO

enum GAMESTATUS {
  notstarted,
  started,
  lost
};

extern enum GAMESTATUS gameStatus;

#define HANDGRIPPIN A0
#define ENCODERPINA 2
#define ENCODERPINB 3

#define HANDGRIPDEVNUM 0 //device number for handgrip
#define CRANKDEVNUM 1

#define HANDGRIPACTIVE 1
#define CRANKACTIVE 1

#define STRIPTESTMODE 0 //used for internal testing

#define GAMERESETDELAY 5000 //in milliseconds

#endif