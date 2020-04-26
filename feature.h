//
// feature.h
// kkyoda@riken.jp
//

#ifndef _FEATURE_H_
#define _FEATURE_H_

class feature {
  int c;
  string s;
  string u;

public:
  feature() {c = 0; s = ""; u = "";}
  feature(int i, string j, string k) {
    c = i;
    s = j;
    u = k;
  }

  int getC() {return c;}
  string getS() {return s;}
  string getU() {return u;}
};

#endif
