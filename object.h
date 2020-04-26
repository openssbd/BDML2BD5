//
// object.h
// kkyoda@riken.jp
//

#ifndef _OBJECT_H_
#define _OBJECT_H_

class object {
  int c;
  string s;

public:
  object() {c = 0; s = "";}
  object(int i, string j) {
    c = i;
    s = j;
  }

  int getC() {return c;}
  string getS() {return s;}
};

#endif
