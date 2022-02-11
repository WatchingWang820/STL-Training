#pragma once
#include <iostream>
using namespace std;

class BasicClass {
 public:
  virtual void BasicFunc() { cout << "basic func\n"; }

  int a;
};

class SonAClass : virtual public BasicClass {
 public:
  virtual void SonAFunc() { cout << "son A func\n"; }

  int b;
};

class SonBClass : virtual public BasicClass {
 public:
  virtual void SonBFunc() { cout << "son B func\n"; }

  int c;
};

class SonCClass : public SonBClass, public SonAClass {
 public:
  virtual void SonCFunc() { cout << "son C func\n"; }

  int d;
};