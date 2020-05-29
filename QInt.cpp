#include "QInt.h"

QInt::QInt() {
    memset(bitArr, 0, SIZE);
}

QInt::~QInt() {
}

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

QInt::QInt(const QInt& other) {
    for (int i = SIZE - 1; i >= 0; i--) {
        bitArr[i] = other.bitArr[i];
    }
}

void QInt::setBit(int pos, char val) {
    if (val) {
        bitArr[pos / 8] |= (1 << (7 - pos % 8));
    }
    else {
        bitArr[pos / 8] &= (~(1 << (7 - pos % 8)));
    }
}

char QInt::getBit(int pos) {
    return (bitArr[pos / 8] >> (7 - pos % 8)) & 1;
}


void QInt::fromBin(string str) {
    memset(bitArr, 0, SIZE);
    for (int i = str.size() - 1; i >= 0; --i) {
        int pos = SIZE * 8 - (str.size() - i);
        if (str[i] == '0') setBit(pos, 0);
        else setBit(pos, 1);
    }
}
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
        div2(str);
        if (str.empty()) break;;
    }
    if (isNegative) {
        *this = twoComplement();
    }
}
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

void QInt::div2(string& str) {
    string result;
    char remainder = 0;
    for (int i = 0; i < str.size(); i++) {
        char d = str[i] - '0';
        result += ((d / 2 + remainder) + '0');
        remainder = (d % 2) * 5;
    }

    if (result[0] == '0') result.erase(result.begin());
    str = result;
}
QInt QInt::operator+(const QInt& ot) {
    //a + b
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

QInt QInt::operator+() {
    return *this;
}

QInt QInt::operator-() {
    return twoComplement();
}

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

QInt QInt::operator-(const QInt& ot) {
    QInt b = ot;
    return *this + b.twoComplement();
}

QInt QInt::operator*(const QInt& ot) {
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

QInt QInt::operator/(const QInt& ot) {
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

QInt QInt::abs() {
    if (getBit(0)) {
        return twoComplement();
    }
    return *this;
}

void QInt::rol() {
    int sizeInBits = SIZE * 8;
    int saved = getBit(0);
    for (int i = 0; i < sizeInBits - 1; ++i) {
        setBit(i, getBit(i + 1));
    }
    setBit(sizeInBits - 1, saved);
}

void QInt::ror() {
    int sizeInBits = SIZE * 8;
    char saved = getBit(sizeInBits - 1);
    for (int i = sizeInBits - 1; i > 0; --i) {
        setBit(i, getBit(i - 1));
    }
    setBit(0, saved);
}

string QInt::toString() {
    string str = "";
    for (int i = 0; i < SIZE * 8; i++) {
        str += getBit(i) + '0';
    }
    return str;
}

string QInt::add(string a, string b) {

    string result;

    string min, max;

    if (a.length() > b.length()) {
        max = a;
        min = b;
    }
    else {
        max = b;
        min = a;
    }

    int i = min.length() - 1;
    int j = max.length() - 1;
    int anchor = 0;
    while (i >= 0) {
        int  t = min[i] - 48 + max[j] - 48 + anchor;
        if (t >= 10) {
            t -= 10;
            anchor = 1;
        }
        else
            anchor = 0;

        char c = 48 + t;
        result = c + result;

        i--;
        j--;

    }

    i = max.length() - min.length() - 1;
    while (i >= 0) {
        int  t = max[i] - 48 + anchor;
        if (t >= 10) {
            t -= 10;
            anchor = 1;
        }
        else
            anchor = 0;

        char c = 48 + t;
        result = c + result;

        i--;


    }

    if (anchor)
        result = "1" + result;


    return result;
}

string QInt::toDec() {

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

            if (c == 1) {
                result = add(add(result, result), "1");
            }
            else
                result = add(add(result, result), "0");
        }
    }

    if (isNegative)
        result = "-" + result;


    return result;

}

string QInt::removeRedundant(string result) {
    //eleminate redundant ahead
    int index = 0;
    while (result[index] == '0')
        index++;

    result = result.substr(index);

    return result;
}


string QInt::toHex() {

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

    string result;

    int n = SIZE - 1;//index of element in array
    int pos = 0;//position of point to bit

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
