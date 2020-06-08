#include "QInt.h"

//Hàm khởi tạo QInt không tham số
QInt::QInt() {
	memset(bitArr, 0, SIZE);
}

//Hàm hủy QInt
QInt::~QInt() {
}

//Khởi tạo QInt với giá trị là chuỗi str, hệ base
QInt::QInt(string str, string base) {
    
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

//Hàm sao chép QInt
QInt::QInt(const QInt& other) {
	for (int i = SIZE - 1; i >= 0; i--) {
		bitArr[i] = other.bitArr[i];
	}
}

//Thay đổi giá trị bit thứ pos với giá trị 

void QInt::setBit(int pos, char val) {
	if (val) {
		bitArr[pos / 8] |= (1 << (7 - pos % 8));
	}
	else {
		bitArr[pos / 8] &= (~(1 << (7 - pos % 8)));
	}
}

//Lấy giá trị bit thứ pos

char QInt::getBit(int pos) {
	return (bitArr[pos / 8] >> (7 - pos % 8)) & 1;
}


//Khởi tạo giá trị QInt từ chuỗi str, hệ 2

void QInt::fromBin(string str) {
	memset(bitArr, 0, SIZE);
	for (int i = str.size() - 1; i >= 0; --i) {
		int pos = SIZE * 8 - (str.size() - i);
		if (str[i] == '0') setBit(pos, 0);
		else setBit(pos, 1);
	}
}

//Khởi tạo giá trị QInt từ chuỗi str, hệ 10

void QInt::fromDec(string str) {
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

//Khởi tạo giá trị QInt từ chuỗi str, hệ 16

void QInt::fromHex(string str) {
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

//Trả về giá trị chuỗi str(hệ 2) chia 2

string QInt::div2(string str) {
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

//Phép cộng 2 số (*this + ot)

QInt QInt::operator+(const QInt& ot) const{
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

//Phép bù 2

QInt QInt::twoComplement() {
	QInt result;
	for (int i = 0; i < SIZE * 8; ++i) {
		result.setBit(i, getBit(i) ^ 1);
	}
	QInt a1;
	a1.setBit(SIZE * 8 - 1, 1);
	result = result + a1;
	return result;
}

//Phép trừ 2 số (*this - ot)

QInt QInt::operator-(const QInt& ot) const{
	QInt b = ot;
	return *this + b.twoComplement();
}

//Phép nhân 2 số (*this * ot)

QInt QInt::operator*(const QInt& ot) const{
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

//Phép chia 2 số (*t)

QInt QInt::operator/(const QInt& ot) const{
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

//Tính giá trị tuyệt đối

QInt QInt::abs() {
	if (getBit(0)) {
		return twoComplement();
	}
	return *this;
}

//Phép xoay trái

void QInt::rol() {
	int sizeInBits = SIZE * 8;
	int saved = getBit(0);
	for (int i = 0; i < sizeInBits - 1; ++i) {
		setBit(i, getBit(i + 1));
	}
	setBit(sizeInBits - 1, saved);
}

//Phép xoay phải

void QInt::ror() {
	int sizeInBits = SIZE * 8;
	char saved = getBit(sizeInBits - 1);
	for (int i = sizeInBits - 1; i > 0; --i) {
		setBit(i, getBit(i - 1));
	}
	setBit(0, saved);
}


//Nhân 2 kết quả a và cộng với x

string QInt::mul2(string a, char x) {
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

//Chuyển từ QInt sang hệ 10

string QInt::toDec() {

	bool isNegative = false;
	int bit = (bitArr[0] >> 7) & 1;
	if (bit == 1) {
		*this = twoComplement();
		isNegative = true;
	}

	string result = "0";


	for (int i = 0; i < SIZE * 8; i++) {
		char c = getBit(i);

		result = mul2(result, c);
	}

	if (isNegative)
		result = "-" + result;


	return result;

}

//bỏ tất cả các số 0 thừa k có giá trị phía trước

string QInt::removeRedundant(string result) {
	int index = 0;
	while (result[index] == '0')
		index++;

	result = result.substr(index);

	if (result.empty())
		result = "0";

	return result;
}


//Chuyển từ QInt sang hệ 16

string QInt::toHex() {



	int n = SIZE - 1;
	int pos = 0;

	string result;
	
	int c = 0;
	for (int i = 0, j = 3; i < SIZE * 8; i++, j--) {
		int bit = getBit(i);

		if (bit == 1)
			c = (1 << j) | c;
		else
			c = ~(1 << j) & c;

		if ((i + 1) % 4 == 0) {

			if (c >= 10)
				result += (char)(c - 10 + 65);
			else
				result += (char)(c + 48);

			c = 0;
			j = 4;
		}
	}

	result = removeRedundant(result);
	

	return result;

}



//Chuyển từ QInt sang hệ 10

string QInt::toBin() {



	string result;

	int n = SIZE - 1;
	int pos = 0;

	for (int i = 0; i < SIZE * 8; i++) {
		int bit = getBit(i);
		char c = 48 + bit;
		result += c;
	}

	result = removeRedundant(result);

	return result;
}


//Trả vê giá trị lưu trữ theo hệ base

string QInt::toString(string base) {
	if (base == "2")
		return toBin();
	else if (base == "16")
    	return toHex();
    return toDec();
}
