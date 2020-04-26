//
// objectentity.h
// kkyoda@riken.jp
//

#ifndef _OBJECTENTITY_H_
#define _OBJECTENTITY_H_

#include "xyzt.h"
#include <list>
#include <cstring>
using namespace std;

class objectentity {
  int objnum;
  string entitytype;
  list<xyzt> point;
  list<xyzt> circle;
  list<xyzt> sphere;
  list<xyzt> line;
  list<xyzt> face;
  list<xyzt> object;

public:
  objectentity() {objnum = 0; entitytype = ""; point.clear(); circle.clear(); sphere.clear(); line.clear(); face.clear();}

  string getS() {return entitytype;}
  int getON() {return objnum;}
  list<xyzt> getPoint() {return point;}
  list<xyzt> getCircle() {return circle;}
  list<xyzt> getSphere() {return sphere;}
  list<xyzt> getLine() {return line;}
  list<xyzt> getFace() {return face;}
  list<xyzt> getObject() {return object;}

  void addObject(xyzt i) {
    object.push_back(i);
  }

  void addPoint(xyzt i) {
    point.push_back(i);
  }

  void addLine(xyzt i) {
    line.push_back(i);
  }

  void addCircle(xyzt i) {
    circle.push_back(i);
  }

  void addSphere(xyzt i) {
    sphere.push_back(i);
//    cerr << "s: " << i.getX() << endl;
  }

  void addFace(xyzt i) {
    face.push_back(i);
  }

  void setObject(int on, xyzt i) {
    objnum = on;
    entitytype = "object";
    object.push_back(i);
  }

  void setPoint(int on, xyzt i) {
    objnum = on;
    entitytype = "point";
    point.push_back(i);
  }

  void setLine(int on, xyzt i) {
    objnum = on;
    entitytype = "line";
    line.push_back(i);
  }

  void setCircle(int on, xyzt i) {
    objnum = on;
    entitytype = "circle";
    circle.push_back(i);
  }

  void setSphere(int on, xyzt i) {
    objnum = on;
    entitytype = "sphere";
    sphere.push_back(i);
//    cerr << i.getX() << endl;
//    cerr << "add sphere: " << on << endl;
  }

  void setFace(int on, xyzt i) {
    objnum = on;
    entitytype = "face";
    face.push_back(i);
  }
};

#endif
