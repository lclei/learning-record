#include <iostream>
using namespace std;

class foo{
public:
  explicit foo(string s): str(s) {}
  void add(const foo &f){str += f.str;}
  void print(){cout << str;}
private:
  string str;
};

int main(){
  string s = "something";
  foo f("here");
  f.add(s);
  f.print();
}
