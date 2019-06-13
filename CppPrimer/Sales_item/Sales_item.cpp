#include <iostream>
#include <string>
#include "Sales_item.h"
using namespace std;

Sales_data &Sales_data::combine(const Sales_data &rhs){
  units_sold += rhs.units_sold;
  revenue += rhs.revenue;
  return *this;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs){
  Sales_data sum = lhs;
  sum.combine(rhs);
  return sum;
}

istream &read(istream &is, Sales_data &item){
  double price = 0.0;
  is >> item.bookNo >> item.units_sold >> price;
  item.revenue = price * item.units_sold;
  return is;
}

ostream &print(ostream &os, const Sales_data &item){
  os << item.isbn() << " " << item.units_sold << " "
    << item.revenue << " " << item.avg_price();
  return os;
}

Sales_data::Sales_data(istream &is){
  read(is, *this);
}

int main(int argc, char *argv[]){
  cout << "Three constructors" << endl;
  Sales_data Sales_item1,Sales_item2("978-7-111-49600-7",1,79.00),Sales_item3(cin);
  print(cout,Sales_item1);
  cout << endl;
  print(cout,Sales_item2);
  cout << endl;
  print(cout,Sales_item3);
  cout << endl;
  cout << "Test add function" << endl;
  Sales_item1 = add(Sales_item2, Sales_item3);
  print(cout,Sales_item1);
  cout << endl;

}
