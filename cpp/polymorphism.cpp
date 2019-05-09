#include <iostream>
using namespace std;

class Animal{
public:
  virtual void sound(){
    cout << "??" << endl;
  }
};

class Cat : public Animal{
public:
  void sound(){
    cout << "Miao Miao" << endl;
  }
};

class Dog : public Animal{
public:
  void sound(){
    cout << "Wang Wang" << endl;
  }
};

int main(int argc, char **argv){
  Animal *animal;
  animal = new Cat;
  animal->sound();
  animal = new Dog;
  animal->sound();
}
