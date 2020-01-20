#ifndef OBJECTS_CPP
#define OBJECTS_CPP

#include "superclasses.cpp"
#include "lcd.cpp"
#include "handgrip.cpp"
#include "encoder.cpp"
#include "indicatorstrip.cpp"
#include "menu.cpp"

class _objects {
  public:
    _objects();
    _device *DCON1_ptr;
    _device *ACON1_ptr;
    _device *DACON1_ptr;
    _device *DCON2_ptr;
    _device *ACON2_ptr;
    _device *DACON2_ptr;
    _device *gen_ptr;
    //CLASS METHODS----------------------
    _device* createObject(int objtype, int portnum){
      if(objtype == LCD_TYPE){
        gen_ptr = new _lcd;
        return(gen_ptr);
      }

      if(objtype == MENU_TYPE){
        gen_ptr = new _menu;
        return(gen_ptr);
      }

      if(objtype == INDICATORSTRIP_TYPE){
        gen_ptr = new _indicatorstrip; 
        return(gen_ptr);
      }

      if(objtype == HANDGRIP_TYPE){
        if(portnum == ACON1_PORTNUM){
          ACON1_ptr = new _handgrip(ACON1_PORTNUM)
        }
        if(portnum == ACON2_PORTNUM){
          
        }
        if(portnum == DACON1_PORTNUM){
          
        }
        if(portnum == ACON2_PORTNUM){
          
        }
        gen_ptr = new _handgrip;
        return(gen_ptr);
      }

      // if(objtype == ENCODER_TYPE){
      //   gen_ptr = new _encoder((_indicatorstrip*)indicatorstrip_ptr);

      //   if(portnum == DCON1_PORTNUM){
      //     ((_encoder*)gen_ptr)->encoderpina = 3;
      //     ((_encoder*)gen_ptr)->encoderpinb = 5;
      //   }
      //   if(portnum == DCON2_PORTNUM){
      //     ((_encoder*)gen_ptr)->encoderpina = 9;
      //     ((_encoder*)gen_ptr)->encoderpinb = 10;
      //   }
      //   // enable pin change interrupt for encoder pin A
      //   *digitalPinToPCMSK(((_encoder*)gen_ptr)->encoderpina) |= 
      //     bit(digitalPinToPCMSKbit(((_encoder*)gen_ptr)->encoderpina)); 

      //   // enable ping change interrupt for encoder pin B
      //   *digitalPinToPCMSK(((_encoder*)gen_ptr)->encoderpinb) |= 
      //     bit(digitalPinToPCMSKbit(((_encoder*)gen_ptr)->encoderpinb)); 

      //   // clear any outstanding pin change interrupt flags
      //   PCIFR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpina)); 
      //   PCIFR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpinb));

      //   // enable interrupt for the GROUP (digital pins 1-7, digtial pins 8-13)
      //   PCICR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpina)); 
      //   PCICR  |= bit (digitalPinToPCICRbit(((_encoder*)gen_ptr)->encoderpinb));

      //   pinMode(((_encoder*)gen_ptr)->encoderpinb, INPUT_PULLUP);
      //   pinMode(((_encoder*)gen_ptr)->encoderpina, INPUT_PULLUP);
      // }
      return(gen_ptr);
    }
    void deleteObject(int port){

    }
};

#endif