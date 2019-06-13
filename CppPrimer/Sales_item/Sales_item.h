// Sales_data class definition
// C++ Primer fifth edition
// All hand typed by LCL
// 20190601
#ifndef _SALES_ITEM_H
#define _SALES_ITEM_H

using namespace std;

class Sales_data{
	// friend makes non-menber functions able to visit menbers
	// friend declaration is not a real declaration
	// The function need be declared formly
friend Sales_data add(const Sales_data&, const Sales_data&);
friend istream &read(istream&, Sales_data&);
friend ostream &print(ostream&, const Sales_data&);

public:
	Sales_data() = default;
	Sales_data(const string &s, unsigned n, double p):
			bookNo(s), units_sold(n), revenue(p*n) {}
	Sales_data(const string &s):bookNo(s){}
	Sales_data(istream&);
	string isbn() const {return bookNo;}
	Sales_data &combine(const Sales_data&);
private:
	double avg_price() const { return units_sold ? revenue/units_sold : 0;}
	string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

// functions declaration
Sales_data add(const Sales_data&, const Sales_data&);
istream &read(istream&, Sales_data&);
ostream &print(ostream&, const Sales_data&);


#endif
