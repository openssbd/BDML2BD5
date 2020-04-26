//
// fromto.h
// kkyoda@riken.jp
//

#ifndef _FROMTO_H_
#define _FROMTO_H_

class fromto {
  string from;
  string to;

public:
  fromto() {from = ""; to = "";}
  fromto(string a, string b) {
    from = a;
    to = b;
  }

  string getFrom() {return from;}
  string getTo() {return to;}
};

#endif
