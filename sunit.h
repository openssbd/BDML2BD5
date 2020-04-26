//
// sunit.h
// kkyoda@riken.jp
//

#ifndef _SUNIT_H
#define _SUNIT_H

class sunit {
  string s;
  double xs;
  double ys;
  double zs;
  string xyzu;
  double ts;
  string tu;

public:
  sunit() {s = ""; xs = ys = zs = ts = 0.0; xyzu = ""; tu = "";}
  sunit(string i, double x, double y, double z, string j, double t, string k) {
    s = i;
    xs = x;
    ys = y;
    zs = z;
    xyzu = j;
    ts = t;
    tu = k;
  }

  string getD() {return s;}
  double getX() {return xs;}
  double getY() {return ys;}
  double getZ() {return zs;}
  string getXYZU() {return xyzu;}
  double getT() {return ts;}
  string getTU() {return tu;}
};

#endif
