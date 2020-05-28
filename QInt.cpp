#include "QInt.h"

QInt::QInt() {

}


void QInt::binToQInt(string input) {

		int n = SIZE - 1;//index of element in array
		int pos = 0;//position of point to bit

		int i = input.length() - 1;
		while (i >= 0) {
			char bit = input[i];
			
			
			//set bit
			if (bit == '1') {
				bitArr[n] = (1 << pos) | bitArr[n];
			}
			else {
				bitArr[n] = ~(1 << pos) & bitArr[n];
			}

			i--;
			pos++;
			//out of 8 number bit, move on the next one
			if (pos > 7) {
				pos = 0;
				n--;
			}
			
		}

	}
	
string QInt::div2(string number) {

		int divisor = 2;

		string result;

		

		int i = 0;
		int division = number[i++] - 48;

		//check if division as first number is smaller than divisor
		int length = number.length();

		while (division < divisor && i < length) {
			division = division * 10 + (number[i++] - 48);
		}
		if (division < divisor) {
			result = "0";
			return result;
		}


		
		while (i <= length) {
			
			char c;
			c = 48 + (division / divisor);
			result += c;
			division = (division % divisor) * 10 + (number[i++] - 48);

		}

		return result;
	}
	
	
void QInt::decToQInt(string input) {
		
		//check if it is a negative number
		bool isNegative = false;
		if (input[0] == '-') {
			input = input.substr(1);
			isNegative = true;
		}
			

		int n = SIZE - 1;//index of element in array
		int pos = 0;//position of point to bit

		
		while (input != "0") {
			int i = input.length() - 1;
			//mod 2
			int mod = (input[i] - 48) % 2;
			
			//set bits
			if (mod == 1) {
				bitArr[n] = (1 << pos) | bitArr[n];
			}
			else {
				bitArr[n] = ~(1 << pos) & bitArr[n];
			}
			
			//div 2
			input = div2(input);

			pos++;
			//out of 8 number bit, move on the next one
			if (pos > 7) {
				pos = 0;
				n--;
			}

		}

		//process negetive number
		if (isNegative) {
			n = SIZE - 1;
			pos = 0;
			int index;
			
			//find the first index from right to left whose value is 1
			while (n >= 0) {

				index = (bitArr[n] >> pos) & 1;
				
				if (index == 1) {
					pos++;
					if (pos > 7) {
						pos = 0;
						n--;
					}
					break;
				}

				pos++;
				//out of 8 number bit, move on the next one
				if (pos > 7) {
					pos = 0;
					n--;
				}
			}

			//exchange bits ahead desired index
			while (n >= 0) {

				int bit = (bitArr[n] >> pos) & 1;

				if(bit == 1)
					bitArr[n] = ~(1 << pos) & bitArr[n];
				else
					bitArr[n] = (1 << pos) | bitArr[n];

				pos++;
				//out of 8 number bit, move on the next one
				if (pos > 7) {
					pos = 0;
					n--;
				}
			}

			
		}
		
	}
	
	
	int QInt:handleBit(char bit) {
		int result = 0;

		if (bit >= 48 && bit <= 57)
			result = 0 + (bit - 48);
		else if (bit >= 65 && bit <= 70){
			result = 10 + (bit - 65);
		}

		return result;
	}
	
	
void QInt::hexToQInt(string input) {

		int n = SIZE - 1;//index of element in array
		int pos = 0;//position of point to bit

		string result;

		int i = input.length() - 1; 
		while (i >= 0) {
			result = "";
			int value = handleBit(input[i]);

			if (value == 0)
				result = "0";
			else {
				while (value > 0) {
					char c;
					c = 48 + (value % 2);

					result = c + result;
					value /= 2;
				}
			}

			

			//process coefficient 16 with length is 4
			//concatenate bit 0 ahead
			int length = result.length();
			if (length < 4) {
				for (int j = 0; j < 4 - length; j++) {
					result = "0" + result;
				}
			}

			int j = 3;
			while (j >= 0) {
				
				
				//set bit
				if (result[j] == '1') {
					bitArr[n] = (1 << pos) | bitArr[n];
				}
				else {
					bitArr[n] = ~(1 << pos) & bitArr[n];
				}

				j--;
				pos++;
				//out of 8 number bit, move on the next one
				if (pos > 7) {
					pos = 0;
					n--;
				}
			}

			i--;
		}

	}
	
	
QInt::QInt(string input, int coefficient){
		//init array
		for (int i = 0; i < SIZE; i++)
			bitArr[i] = 0;
		
		//convert input to QInt
		switch (coefficient)
		{
		case 2:
			binToQInt(input);
			break;
		case 10:
			decToQInt(input);
			break;
		case 16:
			hexToQInt(input);
			break;
		default:
			break;
		}
}

//display array of bits to screen
	void QInt::displayQInt() {
		for (int i = 0; i < SIZE; i++) {
			char element = bitArr[i];

			for (int j = 7; j >= 0; j--) {
				int bit = (element >> j) & 1;
				cout << bit;
			}
		}
	}


QInt::~QInt() {

}
