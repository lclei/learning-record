// exceetion.h
// 定义异常类
// lcl 20190521
#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <iostream>
using namespace std;

class illegalParameterValue
{
public:
  illegalParameterValue():
  message("Illegal parameter value"){}
  illegalParameterValue(char* theMessage)
  {
    message = theMessage;
  }
  void outputMessage() {cout << message << endl;}
private:
  string message;
};

class NoMem{
public:
  NoMem(){}
};

void my_new_handler(){
  throw NoMem();
};

new_handler Old_Handler_=set_new_handler(my_new_handler);

class OutOfBounds{
public:
  OutOfBounds(){cout << "Out Of Bounds!" << endl;}
};

#endif
