#include <iostream>
#include <string>
#include "exception.h"
using namespace std;

typedef enum {eplus, eminus} signType;


class currency
{
public:
  currency(signType theSign = eplus,
           unsigned long theDollars = 0,
           unsigned int theCents = 0);
  ~currency() {}
  void setValue(signType, unsigned long, unsigned int);
  void setValue(double);
  signType getSign() const {return sign;}
  unsigned long getDollars() const {return dollars;}
  unsigned int getCents() const {return cents;}
  currency add(const currency&) const;
  currency& increment(const currency&);
  void output() const;
private:
  signType sign;
  unsigned long dollars;
  unsigned int cents;
};

currency::currency(signType theSign, unsigned long theDollars, unsigned int theCents)
{
  setValue(theSign, theDollars, theCents);
}

void currency::setValue(signType theSign, unsigned long theDollars, unsigned int theCents)
{
  if (theCents>99)
  throw illegalParameterValue("Cents shall be < 100");

  sign = theSign;
  dollars = theDollars;
  cents = theCents;
}

void currency::setValue(double theAmount)
{
  if (theAmount < 0) {
    sign = eminus;
    theAmount = -theAmount;
  }
  else sign = eplus;
  dollars = (unsigned long)theAmount;
  cents = (unsigned int) ((theAmount-dollars+0.001)*100);
}

currency currency::add(const currency& x) const
{
  long a1, a2, a3;
  currency result;

  a1 = dollars*100 + cents;
  if(sign == eminus) a1 = -a1;

  a2 = x.dollars*100 + x.cents;
  if(x.sign == eminus) a2 = -a2;

  a3 = a1 + a2;

  if(a3 < 0) {result.sign = eminus; a3 = -a3;}
  else result.sign = eplus;
  result.dollars = a3/100;
  result.cents = a3 - result.dollars*100;

  return result;
}

currency& currency::increment( const currency& x)
{
  *this = add(x);
  return *this;
}

void currency::output() const
{
  if(sign == eminus) cout << "-";
  cout << "$" << dollars <<'.';
  if(cents<10) cout << '0';
  cout << cents;
}

int main()
{
  currency g,h(eplus, 3, 50), i, j;

  g.setValue(eminus, 2, 25);
  i.setValue(-6.45);

  j = h.add(g);
  h.output();
  cout << " + ";
  g.output();
  cout << " = ";
  j.output();
  cout << endl;

  j = i.add(g).add(h);
  i.output();
  cout << " + ";
  g.output();
  cout << " + ";
  h.output();
  cout << " = ";
  j.output();
  cout << endl;

  j = i.increment(g);
  cout << "i incremnt:";
  i.output();
  cout << endl;

  cout << "Attempting to intialize with cents = 152" << endl;
  try{i.setValue(eplus,3,152);}
  catch (illegalParameterValue e){
    cout << "Caught throw exception" << endl;
    e.outputMessage();
  }
  return 0;
}
