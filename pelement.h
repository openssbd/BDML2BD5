//
// pelement.h
// kkyoda@riken.jp
//

#ifndef _PELEMENT_H_
#define _PELEMENT_H_

class pelement {
  string id;
  int fid;
  double val;

public:
  pelement() {id = ""; fid = 0; val = 0.0;}

  pelement(string a, int b, double c) {
    id = a;
    fid = b;
    val = c;
  }

  string getId() {return id;}
  int getFId() {return fid;}
  double getVal() {return val;}
};

#endif
