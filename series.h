//
// series.h
// kkyoda@riken.jp
//


#ifndef _SERIES_H
#define _SERIES_H

#include <cstring>
#include <list>
#include "xyzt.h"
#include "property.h"
#include "objectentity.h"
using namespace std;

class series {
  double t;
  list<objectentity> oe;
  list<property> pr;

public:
  series() {t = 0.0; oe.clear(); pr.clear();}

  void setT(double tt) {t = tt;}
  double getT() {return t;}

  void addObject(int on, xyzt i) {
    list<objectentity>::iterator p;
//    int flag = 0;
    for (p = oe.begin(); p != oe.end(); p++) {
      if (p->getON() == on) {
        p->addObject(i);
//        flag = 1;
      }
    }
    objectentity tmp_oe;
    tmp_oe.setObject(on, i);
    oe.push_back(tmp_oe);
  }

  void addPoint(int on, xyzt i) {
    list<objectentity>::iterator p;
    int flag = 0;
    for (p = oe.begin(); p != oe.end(); p++) {
      if (p->getON() == on) {
        p->addPoint(i);
        flag = 1;
      }
    }
    if (flag == 0) {
      objectentity tmp_oe;
      tmp_oe.setPoint(on, i);
      oe.push_back(tmp_oe);
    }
  }

  void addLine(int on, xyzt i) {
    list<objectentity>::iterator p;
    int flag = 0;
    for (p = oe.begin(); p != oe.end(); p++) {
      if (p->getON() == on) {
        p->addLine(i);
        flag = 1;
      }
    }
    if (flag == 0) {
      objectentity tmp_oe;
      tmp_oe.setLine(on, i);
      oe.push_back(tmp_oe);
    }
  }

  void addCircle(int on, xyzt i) {
    list<objectentity>::iterator p;
    int flag = 0;
    for (p = oe.begin(); p != oe.end(); p++) {
      if (p->getON() == on) {
        p->addCircle(i);
        flag = 1;
      }
    }
    if (flag == 0) {
      objectentity tmp_oe;
      tmp_oe.setCircle(on, i);
      oe.push_back(tmp_oe);
    }
  }

  void addSphere(int on, xyzt i) {
    list<objectentity>::iterator p;
    int flag = 0;
    for (p = oe.begin(); p != oe.end(); p++) {
      if (p->getON() == on) {
        p->addSphere(i);
        flag = 1;
        break;
      }
    }
    if (flag == 0) {
      objectentity tmp_oe;
      tmp_oe.setSphere(on, i);
      oe.push_back(tmp_oe);
    }
  }

  void addFace(int on, xyzt i) {
    list<objectentity>::iterator p;
    int flag = 0;
    for (p = oe.begin(); p != oe.end(); p++) {
      if (p->getON() == on) {
        p->addFace(i);
        flag = 1;
        break;
      }
    }
    if (flag == 0) {
      objectentity tmp_oe;
      tmp_oe.setFace(on, i);
      oe.push_back(tmp_oe);
    }
  }

  void addProperty(int on, pelement pe) {
    list<property>::iterator p;
    int flag = 0;
    for (p = pr.begin(); p != pr.end(); p++) {
      if (p->getON() == on) {
        p->addPElement(pe);
        flag = 1;
        break;
      }
    }
    if (flag == 0) {
      property tmp_pr;
      tmp_pr.setPElement(on, pe);
      pr.push_back(tmp_pr);
    }
  }

  list<objectentity> getOE() {return oe;}
  list<property> getPR() {return pr;}
};

#endif
