#pragma once
#include <iostream>
#include <string>

using namespace std;

#define SIZE 16

class QInt
{
private:
	char bitArr[SIZE];
	
	//helper function
	int handleBit(char bit);
	string div2(string number);
	
	void binToQInt(string input);
	void decToQInt(string input);
	void hexToQInt(string input);
	
	
	//display array of bits to screen
	void displayQInt();

public:
	QInt();
	QInt(string input, int coefficient);
	~QInt();
	
	//display array of bits to screen
	void displayQInt();

};

