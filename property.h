//
// property.h
// kkyoda@riken.jp
//

#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include "pelement.h"
using namespace std;

class property {
  int objnum;
  list<pelement> pl;

public:
  property() {pl.clear();}

  int getON() {return objnum;}
  void addPElement(pelement a) {
    pl.push_back(a);
  }

  void setPElement(int on, pelement a) {
    objnum = on;
    pl.push_back(a);
  }

  list<pelement> getPL() {return pl;}
  
};

#endif
