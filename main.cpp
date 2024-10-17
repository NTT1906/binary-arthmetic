#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include <iostream>

using namespace std;

#define BITSET_SIZE_8 8
#define BITSET_SIZE_16 (2 * BITSET_SIZE_8)

char *toBitset(char x) {
	char *c = new char[BITSET_SIZE_8];
	for (int i = 0; i < BITSET_SIZE_8; i++) {
		c[i] = x >> i & 1;
	}
	return c;
}

char *toBitset(short x) {
	char *c = new char[BITSET_SIZE_16];
	for (int i = 0; i < BITSET_SIZE_16; i++) {
		c[i] = x >> i & 1;
	}
	return c;
}

char to8BitNum(const char *bitset) {
	char x{};
	for (int i = 0; i < BITSET_SIZE_8; ++i)
		if ((int)bitset[i] > 0)
			x |= (1 << i);

	return x;
}

short to16BitNum(const char *bitset) {
	short x{};
	for (int i = 0; i < BITSET_SIZE_16; ++i)
		if ((int)bitset[i] > 0)
			x |= (1 << i);

	return x;
}

void resetBitset(char *bitset) {
	for (int i = 0; i < BITSET_SIZE_8; i++)
		bitset[i] = 0;
}

void copyBitset(const char *bitset1, char *bitset2, int n = BITSET_SIZE_8) {
	for (int i = 0; i < BITSET_SIZE_8; i++)
		bitset2[i] = bitset1[i];
}

char *getBitsetCopy(const char *bitset, int n = BITSET_SIZE_8) {
	char *nBs = new char[n]{};
	copyBitset(bitset, nBs, n);
	return nBs;
}

void printBitset(const char *bitset, int n = BITSET_SIZE_8) {
	for (int i = n - 1; i >= 0; i--) {
		if (i % BITSET_SIZE_8 == 7) cout << ' ';
		cout << (((int) bitset[i] > 0) ? 1 : 0);
	}
	cout << " : ";
	switch (n) {
		case BITSET_SIZE_8:
			cout << (int) to8BitNum(bitset);
			break;
		case BITSET_SIZE_16:
			cout << (int) to16BitNum(bitset);
			break;

	}
}

char *add(const char *bitset1, const char *bitset2) {
	char *result = new char[BITSET_SIZE_8]{};
	bool carry = false;
	for (int i = 0; i < BITSET_SIZE_8; i++) {
		result[i] = (bitset1[i] ^ bitset2[i] ^ carry);
		carry = (bitset1[i] & bitset2[i]) || (carry && (bitset1[i] ^ bitset2[i]));
	}
	return result;
}

//char *get2Complement(const char *bitset, int n = BITSET_SIZE_8) {
//	char *result = new char[n];
//	bool electricBoogaloo = false;
//	for (int i = 0; i < n; i++) {
//		if (!electricBoogaloo && result[i]) electricBoogaloo = true;
//		result[i] = electricBoogaloo ? bitset[bitset] ^ 1 : bitset[i];
//	}
//	return result;
//}

//char *get2Complement(const char *bitset, int n = BITSET_SIZE_8) {
//	char *result = new char[n]{};
//	int i = 0;
//	while (i < n) {
//		result[i] = bitset[i];
//		if (bitset[i]) {
//			break;
//		}
//		++i;
//	}
//	++i;
//	while (i < n) {
//		result[i] = bitset[i] ^ 1;
//		++i;
//	}
//	return result;
//}

char *get2Complement(const char *bitset, int n = BITSET_SIZE_8) {
	char *result = new char[n]{};
	for (int i = 0; i < n; i++) result[i] = bitset[i] ^ 1; // Flip bits

	bool carry = true;
	for (int i = 0; carry && i < n; i++) {
		result[i] ^= carry;
		carry = !result[i]; // Correct carry propagation
	}
	return result;
}

//char *get2Complement(const char *bitset, int n = BITSET_SIZE_8) {
////	return toBitset(-(unsigned char) to8BitNum(bitset));
//	char *result = new char[n]{};
//	for (int i = 0; i < n; i++) result[i] = bitset[i] ^ 1;
//
//	bool carry = true;
//	for (int i = 0; carry && i < n; i++) {
//		result[i] ^= carry;
//		carry &= bitset[i] == 0;
//	}
//	return result;
//}


char *sub(char *bitset1, char *bitset2) {
	char *twoComplement = get2Complement(bitset2);
	char *result = add(bitset1, twoComplement);
	delete[] twoComplement;
	return result;
}

void shiftRight(char *bitset, int n) {
	for (int i = 0; i < n - 1; ++i)
		bitset[i] = bitset[i + 1];
	bitset[n - 1] = 0;
}

void shiftLeft(char *bitset, int n) {
	for (int i = n - 1; i > 0; --i)
		bitset[i] = bitset[i - 1];
	bitset[0] = 0;
}

void arithmeticShiftRight(char *bitset, int n) {
	bool neg = bitset[0] != 0;
	for (int i = 0; i < n - 1; ++i)
		bitset[i] = bitset[i + 1];
	bitset[n - 1] = neg;
}

char *mul(char *bitset1, char *bitset2) {
	char *result = new char[2 * BITSET_SIZE_8];
	char *mul1{nullptr}, *mul2{nullptr};

	// Convert to positive
	if (bitset1[BITSET_SIZE_8 - 1] == 0) mul1 = getBitsetCopy(bitset1);
	else mul1 = get2Complement(bitset1);
	if (bitset2[BITSET_SIZE_8 - 1] == 0) mul2 = getBitsetCopy(bitset2);
	else mul2 = get2Complement(bitset2);

	char *mulPool = new char[2 * BITSET_SIZE_8 + 2]{};
	for (int i = 0; i < BITSET_SIZE_8; ++i) mulPool[i] = mul2[i];

	for (int i = 0; i < BITSET_SIZE_8; ++i) {
		if (mul2[i] != 0) {
			bool carry = false;
			for (int j = BITSET_SIZE_8; j < BITSET_SIZE_8 * 2; ++j) {
				char tmp = mulPool[j];
				char tmp1 = mul1[j - BITSET_SIZE_8];
				mulPool[j] = (mulPool[j] ^ tmp1 ^ carry);
				carry = (tmp1 > 0 && tmp > 0) || (carry == 1 && (tmp1 ^ tmp) == 1);
			}
			mulPool[BITSET_SIZE_8 * 2] ^= carry;
		}
		shiftRight(mulPool, 2 * BITSET_SIZE_8 + 1);
	}
	if ((bitset1[BITSET_SIZE_8 - 1] ^ bitset2[BITSET_SIZE_8 - 1]) == 1) {
		result = get2Complement(mulPool, BITSET_SIZE_16);
	} else {
		for (int i = 0; i < BITSET_SIZE_16; i++) {
			result[i] = mulPool[i];
		}
	}

	delete[] mul1;
	delete[] mul2;
	delete[] mulPool;
	return result;
}

int compare(const char *bitset1, const char *bitset2, int n = BITSET_SIZE_8) {
	for (int i = BITSET_SIZE_8 - 1; i >= 0; --i) {
		if (bitset1[i] ^ bitset2[i]) {
			if (bitset1[i] < bitset2[i]) return -1;
			return 1;
		}
	}
	return 0;
}

void resetBitset(char *bitset, int n = BITSET_SIZE_8) {
	for (int i = 0; i < n; ++i) bitset[i] = 0;
}

char *div(char *bitset1, char *bitset2, char *remainder = nullptr) {
	{
		int firstMeaningBit = 0;
		while (firstMeaningBit < BITSET_SIZE_8) {
			if (bitset2[firstMeaningBit]) break;
			++firstMeaningBit;
		}
		if (firstMeaningBit == BITSET_SIZE_8) {
			printf("Cannot divide by zero\n");
			throw invalid_argument("Cannot divide by zero");
		}
	}
	// convert to positive
	char *dividend = bitset1[BITSET_SIZE_8 - 1] ? get2Complement(bitset1, BITSET_SIZE_8) : getBitsetCopy(bitset1, BITSET_SIZE_8);
	char *divisor = bitset2[BITSET_SIZE_8 - 1] ? get2Complement(bitset2, BITSET_SIZE_8) : getBitsetCopy(bitset2, BITSET_SIZE_8);
	int c = compare(dividend, divisor);
	if (c < 1){
		if (remainder != nullptr) {
			if (c == 0) {
				resetBitset(remainder, BITSET_SIZE_8);
			} else {
				copyBitset(bitset1, remainder);
			}
		}
		delete[] dividend;
		delete[] divisor;
		char *result = new char[BITSET_SIZE_8]{};
		result[0] = c != -1;
		return result;
		// If smaller return 0, if equal return 1. If remainder is provided, return
	}
	int maxPossibleShift = 0;
	for (int i = BITSET_SIZE_8 - 1; i >= 0; --i) {
		if (divisor[i]) break;
		++maxPossibleShift;
	}

	char *quotient = new char[BITSET_SIZE_8]{};
	char *tmpDivisor = new char[BITSET_SIZE_8]{};

	while (true) {  // process loop
		if (compare(dividend, divisor) == -1) {
//			if (remainder != nullptr) {
//				copyBitset(dividend, remainder, BITSET_SIZE_8);
//			}
			break;
		}
		int numShiftLeft = 0;
		copyBitset(divisor, tmpDivisor, BITSET_SIZE_8);
		while (numShiftLeft < maxPossibleShift && compare(tmpDivisor, dividend) < 1) { // shift left loop
			shiftLeft(tmpDivisor, BITSET_SIZE_8);
			++numShiftLeft;
		}
		if (compare(tmpDivisor, dividend) == 1) {
			shiftRight(tmpDivisor, BITSET_SIZE_8);
			--numShiftLeft;
		}

		quotient[numShiftLeft] = 1;

		char *tmp = sub(dividend, tmpDivisor);
		copyBitset(tmp, dividend, BITSET_SIZE_8);
		delete[] tmp;
	}
	delete[] tmpDivisor;
	if (bitset1[BITSET_SIZE_8 - 1] ^ bitset2[BITSET_SIZE_8 - 1]) {
		char *tmp = get2Complement(quotient, BITSET_SIZE_8);
		delete[] quotient;
		quotient = tmp;
	}
	if (remainder != nullptr) {
		char *tmp;
		if (bitset1[BITSET_SIZE_8 - 1]) {
			tmp = get2Complement(dividend);
			copyBitset(tmp, remainder, BITSET_SIZE_8);
			delete[] tmp;
		} else {
			copyBitset(dividend, remainder, BITSET_SIZE_8);
		}
	}
//	if (remainder != nullptr && bitset1[BITSET_SIZE_8 - 1]) {
//		char *tmp = get2Complement(remainder);
//		copyBitset(remainder, tmp, BITSET_SIZE_8);
//		delete[] tmp;
//	}
	delete[] dividend;
	delete[] divisor;
	return quotient;
}

char *mod(char *bitset1, char *bitset2) {
	char *remainder = new char[BITSET_SIZE_8]{};
	delete[] div(bitset1, bitset2, remainder);
	return remainder;
}

void run(char a, char b) {
	char *b1 = toBitset( a);
	char *b2 = toBitset( b);
	cout << "-----------------------";
	cout << "\n[!] B1:";
	printBitset(b1);
	cout << "\n[!] B2:";
	printBitset(b2);
	cout << "\n[ADDITION]";
	cout << "\n[+] b1 + b2 = ";
	char *r1 = add(b1, b2);
	printBitset(r1);
	delete[] r1;
	cout << "\n\n[SUBTRACT]";
	cout << "\n[-] b1 - b2 = ";
	char *r2 = sub(b1, b2);
	printBitset(r2);
	delete[] r2;
	cout << "\n[-] b2 - b1 = ";
	char *r3 = sub(b2, b1);
	printBitset(r3);
	delete[] r3;

	cout << "\n\n[MULTIPLY]";
	cout << "\n[x] b1 x b2 = ";
	char *r4 = mul(b1, b2);
	printBitset(r4, BITSET_SIZE_16);
	delete[] r4;
	cout << "\n[x] b2 x b1 = ";
	char *r5 = mul(b2, b1);
	printBitset(r5, BITSET_SIZE_16);
	delete[] r5;

	cout << "\n\n[DIVIDE]";
	cout << "\n[/] b1 / b2 = ";
	char *remainder = new char[BITSET_SIZE_8]{};
	char *r6 = div(b1, b2, remainder);
	printBitset(r6);
	delete[] r6;
	cout << "\n[%] b1 % b2 = ";
	printBitset(remainder);

//	char *b1_2C = get2Complement(b1, BITSET_SIZE_8);
//	char *b2_2C = get2Complement(b2, BITSET_SIZE_8);
//
//	cout << "\n[/] -b1 / b2 = ";
//	char *r7 = div(b1_2C, b2, remainder);
//	printBitset(r7);
//	delete[] r7;
//	cout << "\n[%] -b1 % b2 = ";
//	printBitset(remainder);
//
//	cout << "\n[/] b1 / -b2 = ";
//	char *r8 = div(b1, b2_2C, remainder);
//	printBitset(r8);
//	delete[] r8;
//	cout << "\n[%] b1 % -b2 = ";
//	printBitset(remainder);
//
//	cout << "\n[/] -b1 / -b2 = ";
//	char *r9 = div(b1_2C, b2_2C, remainder);
//	printBitset(r9);
//	delete[] r9;
//	cout << "\n[%] -b1 % -b2 = ";
//	printBitset(remainder);
//
//	delete[] b1_2C;
//	delete[] b2_2C;

	delete[] remainder;
	delete[] b1;
	delete[] b2;
	cout << "\n\n";
}

void printTest(int a, int b) {
	printf("%i / %i -> %i %i\n", a, b, a / b, a % b);
}

char input(const string &vName) {
	int x;
	do {
		cout << "[!] Input " << vName << ": ";
		cin >> x;
		if (x < -128 || x > 127) {
			cout << "[!] Input is not in range [-128, 127]\n";
		}
	} while (x < -128 || x > 127);
	return (char) x;
}

int main() {
	cout << "[INPUT]\n";
	char n1 = input("B1");
	char n2 = input("B2");
//	char n1{12};
//	char n2{0};
	run(n1, n2);
	return 0;
}

#pragma clang diagnostic pop