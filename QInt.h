#pragma once
#include <iostream>
#include <string>

using namespace std;

#define SIZE 16

class QInt
{
private:
	char bitArr[SIZE];
	char getBit(int pos);
	void setBit(int pos, char val);
	void fromBin(string str);
	void fromDec(string str);
	void fromHex(string str);
	string div2(string str);

	string removeRedundant(string result);
	string add(string a, string b);
public:


	QInt();
	~QInt();
	QInt(string str, string base);
	QInt(const QInt& ot);
	QInt operator+(const QInt& ot) const;
	QInt operator-(const QInt& ot) const;
	QInt operator*(const QInt& ot) const;
	QInt operator/(const QInt& ot) const;
	QInt twoComplement();
	QInt abs();

	//Operator &,|,^,~,>>,<<

	QInt operator&(const QInt& ot) const;
	QInt operator|(const QInt& ot) const;
	QInt operator^(const QInt& ot) const;
	QInt operator~() const;

	QInt operator>>(int num) const;
	QInt operator<<(int num) const;


	void ror();
	void rol();
    
	string toDec();
	string toHex();
	string toBin();
	string toString(string base);

    
};
