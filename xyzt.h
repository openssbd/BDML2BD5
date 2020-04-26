//
// xyzt.h
// kkyoda@riken.jp
//


#ifndef _XYZT_H_
#define _XYZT_H_

#include <cstring>
using namespace std;

class xyzt {
  string id;
  double x;
  double y;
  double z;
  double r;
  double t;
  int seqid;
  string label;
  string entity;

public:
  xyzt() {id = ""; x = y = z = r = 0.0; t = 0.0; seqid = 0, entity = "";}
  xyzt(string d, double i, double j, double k, double rad, double m, int s, string l, string n) {
    id = d;
    x = i;
    y = j;
    z = k;
    r = rad;
    t = m;
    seqid = s;
    label = l;
    entity = n;
  }

  string getId() {return id;}
  double getX() {return x;}
  double getY() {return y;}
  double getZ() {return z;}
  double getR() {return r;}
  double getT() {return t;}
  int getSId() {return seqid;}
  string getL() {return label;} 
  string getE() {return entity;}
};

bool operator<(xyzt &a, xyzt &b)
{
  return a.getT() < b.getT();
}

bool operator==(xyzt &a, xyzt &b)
{
  return a.getT() == b.getT();
}

#endif
