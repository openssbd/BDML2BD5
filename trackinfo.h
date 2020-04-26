//
// trackinfo.h
// kkyoda@riken.jp
//

#ifndef _TRACKINFO_H_
#define _TRACKINFO_H_

#include "fromto.h"
using namespace std;

class trackinfo {
  list<fromto> ft;

public:
  trackinfo() {ft.clear();}

  void addFromTo(string a, string b) {
    fromto tmp_fromto(a, b);
    ft.push_back(tmp_fromto);
  }

  list<fromto> getFT() {return ft;}  
};

#endif
