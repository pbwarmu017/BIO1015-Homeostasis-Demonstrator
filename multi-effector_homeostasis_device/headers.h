//this header is where the objects are instantiated. Import this header into any source in which you want to access the objects. 

#ifndef _HEADERSH
#define _HEADERSH

//class forward declarations

//main pin defines
#include <Arduino.h>
//neopixels
#include <Adafruit_NeoPixel.h>
//RGB LCD Display
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

class Adafruit_RGBLCDShield;
class _handgrip;
class _indicatorstrip;
class _encoder;

//custom library headers
#include "parents.h"
#include "menu.h"
#include "multi-effector_homeostasis_device.h"
#include "ledstrip.h"
#include "handgrip.h"
#include "encoder.h"


#endif