#include "QInt.h"

QInt::QInt() {
	memset(bitArr, 0, SIZE);
}

QInt::~QInt() {
}

QInt::QInt(string str, string base) {
    //Khởi tạo QInt với giá trị là chuỗi str, hệ base
	if (base == "2") {
		fromBin(str);
	}
	else if (base == "10") {
		fromDec(str);
	}
	else {
		fromHex(str);
	}
}

QInt::QInt(const QInt& other) {
	for (int i = SIZE - 1; i >= 0; i--) {
		bitArr[i] = other.bitArr[i];
	}
}

void QInt::setBit(int pos, char val) {
    //Thay đổi giá trị bit thứ pos với giá trị val
	if (val) {
		bitArr[pos / 8] |= (1 << (7 - pos % 8));
	}
	else {
		bitArr[pos / 8] &= (~(1 << (7 - pos % 8)));
	}
}

char QInt::getBit(int pos) {
    //Lấy giá trị bit thứ pos
	return (bitArr[pos / 8] >> (7 - pos % 8)) & 1;
}


void QInt::fromBin(string str) {
    //Khởi tạo giá trị QInt từ chuỗi str, hệ 2
	memset(bitArr, 0, SIZE);
	for (int i = str.size() - 1; i >= 0; --i) {
		int pos = SIZE * 8 - (str.size() - i);
		if (str[i] == '0') setBit(pos, 0);
		else setBit(pos, 1);
	}
}
void QInt::fromDec(string str) {
    //Khởi tạo giá trị QInt từ chuỗi str, hệ 10
	memset(bitArr, 0, SIZE);
	char isNegative = 0;
	if (str[0] == '-') {
		isNegative = 1;
		str.erase(str.begin());
	}
	char bit;
	for (int i = SIZE * 8 - 1; i >= 0; --i) {
		bit = (str[str.size() - 1] - '0') % 2;
		setBit(i, bit);
		str = div2(str);
		if (str.empty()) break;;
	}
	if (isNegative) {
		*this = twoComplement();
	}
}
void QInt::fromHex(string str) {
    //Khởi tạo giá trị QInt từ chuỗi str, hệ 16
	memset(bitArr, 0, SIZE);
	char he;
	for (int i = str.size() - 1; i >= 0; --i) {
		int pos = SIZE * 8 - (str.size() - i) * 4;
		char he = 0;
		if (str[i] < 'A') he = str[i] - '0';
		else he = str[i] - 55;//'A' = 65
		setBit(pos, (he >> 3) & 1);
		setBit(pos + 1, (he >> 2) & 1);
		setBit(pos + 2, (he >> 1) & 1);
		setBit(pos + 3, he & 1);
	}
}

string QInt::div2(string str) {
    //Trả về giá trị chuỗi str(hệ 2) chia 2
	string result;
	char remainder = 0;
	for (int i = 0; i < str.size(); i++) {
		char d = str[i] - '0';
		result += ((d / 2 + remainder) + '0');
		remainder = (d % 2) * 5;
	}

	if (result[0] == '0') result.erase(result.begin());
	return result;
}
QInt QInt::operator+(const QInt& ot) const{
	//Phép cộng 2 số (*this + ot)
	QInt b = ot;
	QInt a = *this;
	QInt result;
	char carry = 0;
	for (int i = SIZE * 8 - 1; i >= 0; i--) {
		char sb = (a.getBit(i) ^ b.getBit(i) ^ carry);
		carry = (a.getBit(i) & b.getBit(i)) | (a.getBit(i) & carry) | (b.getBit(i) & carry);
		result.setBit(i, sb);
	}
	return result;
}

QInt QInt::twoComplement() {
    //Phép bù 2
	QInt result;
	for (int i = 0; i < SIZE * 8; ++i) {
		result.setBit(i, getBit(i) ^ 1);
	}
	QInt a1;
	a1.setBit(SIZE * 8 - 1, 1);
	result = result + a1;
	return result;
}

QInt QInt::operator-(const QInt& ot) const{
    //Phép trừ 2 số (*this - ot)
	QInt b = ot;
	return *this + b.twoComplement();
}

QInt QInt::operator*(const QInt& ot) const{
    //Phép nhân 2 số (*this * ot)
	QInt a;
	QInt m = *this;
	QInt q0 = ot;
	char q1 = 0;
	const int sizeInBits = SIZE * 8;
	for (int k = sizeInBits; k > 0; --k) {

		if ((q0.getBit(sizeInBits - 1) & 1) & (q1 ^ 1)) a = a - m;
		else if ((q0.getBit(sizeInBits - 1) ^ 1) & (q1 & 1)) a = a + m;

		q1 = q0.getBit(sizeInBits - 1);
		for (int i = sizeInBits - 1; i > 0; --i) {
			q0.setBit(i, q0.getBit(i - 1));
		}
		q0.setBit(0, a.getBit(sizeInBits - 1));
		for (int i = sizeInBits - 1; i > 0; --i) {
			a.setBit(i, a.getBit(i - 1));
		}
	}
	return q0;
}

QInt QInt::operator/(const QInt& ot) const{
    //Phép chia 2 số (*t)
	QInt m = ot;
	QInt a;
	QInt q0 = *this;
	bool isNegative = m.getBit(0) ^ q0.getBit(0);

	m = m.abs();
	q0 = q0.abs();
	const int sizeInBits = SIZE * 8;
	for (int k = sizeInBits; k > 0; --k) {
		for (int i = 0; i < sizeInBits - 1; i++) {
			a.setBit(i, a.getBit(i + 1));
		}
		a.setBit(sizeInBits - 1, q0.getBit(0));
		for (int i = 0; i < sizeInBits - 1; i++) {
			q0.setBit(i, q0.getBit(i + 1));
		}
		q0.setBit(sizeInBits - 1, 0);
		a = a - m;
		if (a.getBit(0)) {
			a = a + m;
		}
		else {
			q0.setBit(sizeInBits - 1, 1);
		}
	}
	if (isNegative) return q0.twoComplement();
	return q0;
}

//Operator &,|,^,~

QInt QInt::operator&(const QInt& ot) const {
	QInt result;
	for (int i = 0; i < SIZE; i++) {
		result.bitArr[i] = this->bitArr[i] & ot.bitArr[i];
	}
	return result;
}

QInt QInt::operator|(const QInt& ot) const {
	QInt result;
	for (int i = 0; i < SIZE; i++) {
		result.bitArr[i] = this->bitArr[i] | ot.bitArr[i];
	}
	return result;
}

QInt QInt::operator^(const QInt& ot) const {
	QInt result;
	for (int i = 0; i < SIZE; i++) {
		result.bitArr[i] = this->bitArr[i] ^ ot.bitArr[i];
	}
	return result;
}

QInt QInt::operator~() const {
	QInt result;
	for (int i = 0; i < SIZE; i++) {
		result.bitArr[i] = ~this->bitArr[i];
	}
	return result;
}

//Operator <<,>>

QInt QInt::operator>>(int num) const {
	QInt result = *this;
	//while (num > 0) {
	//	char bit=result.getBit(SIZE * 8 - 1);
	//	for (int i = SIZE - 1; i >= 1; i--) {
	//		result.bitArr[i] = result.bitArr[i] >> 1;
	//		if (result.bitArr[i - 1] & 1) {
	//			result.bitArr[i] = (1 << 7 | result.bitArr[i]);
	//		}
	//	}
	//	result.bitArr[0] = result.bitArr[0] >> 1;
	//	result.setBit(SIZE * 8 - 1, bit);
	//	num--;
	//}
	char saved = result.getBit(0);

	for (int i = 127; i >= num; --i) {
		result.setBit(i, result.getBit(i - num));
	}
	for (int i = 0; i < num; i++) {
		result.setBit(i, saved);
	}
	return result;
}

QInt QInt::operator<<(int num) const {
	QInt result = *this;
	while (num > 0) {
		for (int i = 0; i < SIZE - 1; i++) {
			result.bitArr[i] = result.bitArr[i] << 1;
			if ((result.bitArr[i + 1] >> 7) & 1) {
				result.bitArr[i] = (1 | result.bitArr[i]);
			}
		}
		result.bitArr[SIZE - 1] = result.bitArr[SIZE - 1] << 1;
		num--;
	}
	return result;
}

QInt QInt::abs() {
    //Tính giá trị tuyệt đối
	if (getBit(0)) {
		return twoComplement();
	}
	return *this;
}

void QInt::rol() {
    //Phép xoay trái
	int sizeInBits = SIZE * 8;
	int saved = getBit(0);
	for (int i = 0; i < sizeInBits - 1; ++i) {
		setBit(i, getBit(i + 1));
	}
	setBit(sizeInBits - 1, saved);
}

void QInt::ror() {
    //Phép xoay phải
	int sizeInBits = SIZE * 8;
	char saved = getBit(sizeInBits - 1);
	for (int i = sizeInBits - 1; i > 0; --i) {
		setBit(i, getBit(i - 1));
	}
	setBit(0, saved);
}


string QInt::mul2(string a, int x) {
	//Nhân 2 kết quả a và cộng với x
	string result;

	int redundant = x;
	for (int i = a.length() - 1; i >= 0; i--) {
		int t = (a[i] - 48) * 2 + redundant;
		result = (char)(t % 10 + 48) + result;
		redundant = t / 10;
	}

	if (redundant > 0)
		result = '1' + result;

	return result;
}

string QInt::toDec() {
	//Chuyển từ QInt sang hệ 10

	bool isNegative = false;
	int bit = (bitArr[0] >> 7) & 1;
	if (bit == 1) {
		*this = twoComplement();
		isNegative = true;
	}

	string result = "0";

	for (int i = 0; i < SIZE; i++) {
		char element = bitArr[i];


		for (int j = 7; j >= 0; j--) {

			int c = (element >> j) & 1;

			result = mul2(result, c);
		}
	}

	if (isNegative)
		result = "-" + result;


	return result;

}

string QInt::removeRedundant(string result) {
	//bỏ tất cả các số 0 thừa k có giá trị phía trước
	int index = 0;
	while (result[index] == '0')
		index++;

	result = result.substr(index);

	if (result.empty())
		result = "0";

	return result;
}


string QInt::toHex() {

	//Chuyển từ QInt sang hệ 16


	int n = SIZE - 1;//index of element in array
	int pos = 0;//position of point to bit

	string result;

	int i = 0;
	int c = 0;
	while (n >= 0) {

		int bit = (bitArr[n] >> pos) & 1;

		if (bit == 1)
			c = (1 << i) | c;
		else
			c = ~(1 << i) & c;

		i++;
		if (i == 4) {

			if (c >= 10)
				result = (char)(c - 10 + 65) + result;
			else
				result = (char)(c + 48) + result;

			c = 0;
			i = 0;
		}

		pos++;
		if (pos > 7) {
			n--;
			pos = 0;
		}

	}

	result = removeRedundant(result);
	

	return result;

}


string QInt::toBin() {

	//Chuyển từ QInt sang hệ 10


	string result;

	int n = SIZE - 1;
	int pos = 0;

	while (n >= 0) {

		int bit = (bitArr[n] >> pos) & 1;
		char c = 48 + bit;
		result = c + result;

		pos++;
		if (pos > 7) {
			n--;
			pos = 0;
		}

	}

	result = removeRedundant(result);

	return result;
}


string QInt::toString(string base) {
    //Trả vê giá trị lưu trữ theo hệ base
	if (base == "2")
		return toBin();
	else if (base == "16")
    	return toHex();
    return toDec();
}
