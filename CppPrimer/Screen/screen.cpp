#include <iostream>
#include <vector>
#include <string>
#include "Screen.h"
using namespace std;

int main(int argc, char *argv[]){

  Screen screen1,x(5,80,'x'),blank(5,80);
  Window_mgr win;
  win.addScreen(x);
  win.addScreen(blank);

  cout << "screen1:" << endl;
  screen1.display(cout);
  cout << "screen2: all x" << endl;
  x.display(cout);
  cout << "screen3: blank and set (5,5) *" << endl;
  blank.move(4,5).set('*').display(cout);

  cout << "window1:" << endl;
  win.print(0);
  cout << "window2:" << endl;
  win.print(1);

  return 0;
}
