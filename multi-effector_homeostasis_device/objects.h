#ifndef OBJECTS_H
#define OBJECTS_H

#include "superclasses.cpp"

class _objects{
public:
  _device* createObject(int objtype, int portnum);
  void deleteObject(int port);
};

#endif