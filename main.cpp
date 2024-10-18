#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include <iostream>

using namespace std;

#include <vector>

char *toBitset(char x) {
	char *c = new char[8];
	for (int i = 0; i < 8; i++) {
		c[i] = x >> i & 1;
	}
	return c;
}

char *toBitset(short x) {
	char *c = new char[16];
	for (int i = 0; i < 16; i++) {
		c[i] = x >> i & 1;
	}
	return c;
}

char to8BitNum(const char *bitset) {
	char x{};
	for (int i = 0; i < 8; ++i)
		if ((int)bitset[i] > 0)
			x |= (1 << i);

	return x;
}

short to16BitNum(const char *bitset) {
	short x{};
	for (int i = 0; i < 16; ++i)
		if ((int)bitset[i] > 0)
			x |= (1 << i);

	return x;
}

void copyBitset(const char *bitset1, char *bitset2, int n = 8) {
	for (int i = 0; i < n; i++)
		bitset2[i] = bitset1[i];
}

char *getBitsetCopy(const char *bitset, int n = 8) {
	char *nBs = new char[n]{};
	copyBitset(bitset, nBs, n);
	return nBs;
}

void printBitset(const char *bitset, int n = 8) {
	for (int i = n - 1; i >= 0; i--) {
		if (i % 8 == 7) cout << ' ';
		cout << (((int) bitset[i] > 0) ? 1 : 0);
	}
	cout << " : ";
	if (n == 8) {
		cout << (int) to8BitNum(bitset);
	} else if (n == 16) {
		cout << (int) to16BitNum(bitset);
	}
}

// /* get2Complement, memory effi */
//char *get2Complement(const char *bitset, int n = 8) {
//	char *result = new char[n];
//	bool electricBoogaloo = false;
//	for (int i = 0; i < n; i++) {
//		if (!electricBoogaloo && result[i]) electricBoogaloo = true;
//		result[i] = electricBoogaloo ? bitset[bitset] ^ 1 : bitset[i];
//	}
//	return result;
//}

// /* get2Complement, memory effi & time effi */
char *get2Complement(const char *bitset, int n = 8) {
	char *result = new char[n]{};
	int i = 0;
	while (i < n) {
		result[i] = bitset[i];
		if (bitset[i]) {
			break;
		}
		++i;
	}
	++i;
	while (i < n) {
		result[i] = bitset[i] ^ 1;
		++i;
	}
	return result;
}

// /* get2Complement, good structured? */

//char *get2Complement(const char *bitset, int n = 8) {
//	char *result = new char[n]{};
//	for (int i = 0; i < n; i++) result[i] = bitset[i] ^ 1; // Flip bits
//
//	bool carry = true;
//	for (int i = 0; carry && i < n; i++) {
//		result[i] ^= carry;
//		carry = !result[i]; // Correct carry propagation
//	}
//	return result;
//}

// /* get2Complement, original */

//char *get2Complement(const char *bitset, int n = 8) {
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

int compare(const char *bitset1, const char *bitset2, int n = 8) {
	for (int i = n - 1; i >= 0; --i) {
		if (bitset1[i] != bitset2[i]) { // Nếu bit a và bit b khác dấu thì hoặc a hoặc b nhỏ hơn, thay bitset1[i] != bitset2[i] chắc hoạt động tương tự
			if (bitset1[i] < bitset2[i]) return -1; // bit a nhỏ hơn bit b thì return -1
			return 1;
		}
	}
	// nếu nó tới đây thì mọi bit theo thứ tự trong a, b bằng nhau nên a, b bằng nhau
	return 0;
}

void resetBitset(char *bitset, int n = 8) {
	for (int i = 0; i < n; ++i) bitset[i] = 0; // đẩy mảng về 0
}

char *add(const char *bitset1, const char *bitset2) {
	char *result = new char[8]{};
	bool carry = false;
	for (int i = 0; i < 8; i++) {
		result[i] = (bitset1[i] ^ bitset2[i] ^ carry);
		carry = (bitset1[i] & bitset2[i]) || (carry && (bitset1[i] ^ bitset2[i]));
	}
	return result;
}

char *sub(char *bitset1, char *bitset2) {
	char *twoComplement = get2Complement(bitset2);
	char *result = add(bitset1, twoComplement);
	delete[] twoComplement;
	return result;
}

/**
 * @brief Multiply two number
 * @param bitset1 Input first value
 * @param bitset2 Input second value
 */
char *mul(char *bitset1, char *bitset2) {
	char *result = new char[16];
	// Convert to positive
	char *mul1 = bitset1[7] ? get2Complement(bitset1) : getBitsetCopy(bitset1);
	char *mul2 = bitset2[7] ? get2Complement(bitset2) : getBitsetCopy(bitset2);

	char mulPool[16]{};
	for (int i = 0; i < 8; ++i) mulPool[i] = mul2[i];

	bool cAll = false;

	for (int i = 0; i < 8; ++i) {
		if (mul2[i] != 0) {
			bool carry = false;
			for (int j = 8; j < 16; ++j) {
				char tmp = mulPool[j];
				char tmp1 = mul1[j - 8];
				mulPool[j] = (mulPool[j] ^ tmp1 ^ carry);
				carry = (tmp1 > 0 && tmp > 0) || (carry && (tmp1 ^ tmp));
			}
			cAll ^= carry;
		}
		shiftRight(mulPool, 16);
		mulPool[15] = cAll;
	}
	// Tweak the result to get the correct sign
	if (bitset1[7] ^ bitset2[7]) {
		result = get2Complement(mulPool, 16);
	} else {
		for (int i = 0; i < 16; i++) {
			result[i] = mulPool[i];
		}
	}

	delete[] mul1;
	delete[] mul2;
	return result;
}

/**
 * @brief Long divide algorithm, bitset1 / bitset2
 * @param bitset1 Input dividend
 * @param bitset2 Input divisor
 * @param remainder Output remainder (optional)
 *
 * @example -72 / 20 -> -3, -12
 * @example  25 / -3 -> -8,   1
 * @note if remainder is not set (NULL by default), the algo will continue without giving the remainder of the operator
 */
char *div(char *bitset1, char *bitset2, char *remainder = nullptr) {
	// Check if bitset2 is zero, if yes then throw an exception as "Cannot divide by zero."
	int firstMeaningBit = 0;
	while (firstMeaningBit < 8) {
		if (bitset2[firstMeaningBit]) break;
		++firstMeaningBit;
	}
	if (firstMeaningBit == 8) {
		printf("Cannot divide by zero.\n");
		return nullptr;
//		throw invalid_argument("Cannot divide by zero.");
	}

	// Convert to both bitset1 and bitset2 to their positive counterpart if not.
	char *dividend = bitset1[7] ? get2Complement(bitset1, 8) : getBitsetCopy(bitset1, 8);
	char *divisor = bitset2[7] ? get2Complement(bitset2, 8) : getBitsetCopy(bitset2, 8);
	int c = compare(dividend, divisor);

	char *quotient = new char[8]{};

	if (c > 0) { // If bitset1 is greater than bitset2
		int maxPossibleShift = 0;
		for (int i = 8 - 1; i >= 0; --i) {
			if (divisor[i]) break;
			++maxPossibleShift;
		}
		char tmpDivisor[8];
		copyBitset(divisor, tmpDivisor);

		while (true) {  // process loop
			if (compare(dividend, divisor) == -1) {
				copyBitset(dividend, remainder, 8);
				break;
			}
			int numShiftLeft = 0;
			copyBitset(divisor, tmpDivisor, 8);
			while (numShiftLeft < maxPossibleShift && compare(tmpDivisor, dividend) < 1) { // shift left loop
				shiftLeft(tmpDivisor, 8);
				++numShiftLeft;
			}
			if (compare(tmpDivisor, dividend) == 1) {
				shiftRight(tmpDivisor, 8);
				--numShiftLeft;
			}
			quotient[numShiftLeft] = 1;

			char *tmp = sub(dividend, tmpDivisor);
			copyBitset(tmp, dividend, 8);
			delete[] tmp;
		}
	} else { // else, either bitset1 is equal to bitset2 or smaller, if smaller than the reminder is bitset1
		if (remainder != nullptr) {
			if (c == 0) {
				resetBitset(remainder, 8); // bitset1 = bitset2 -> remainder = 0
			} else {
				copyBitset(bitset1, remainder); // remainder = bitset1, bitset1 < bitset2
			}
		}
		if (c == 0) quotient[0] = 1; // if equal then the remainder is 1
	}

	// Tweak the sign of quotient to their correct value, the quotient sign is the xor result of bitset1 & bitset2
	if (bitset1[8 - 1] ^ bitset2[8 - 1]) {
		char *tmp = get2Complement(quotient, 8);
		delete[] quotient;
		quotient = tmp;
	}
	if (remainder != nullptr) {
		char *tmp;
		// Same as the above but the remainder sign is the sign of bitset1
		if (bitset1[8 - 1]) {
			tmp = get2Complement(dividend);
			copyBitset(tmp, remainder, 8);
			delete[] tmp;
		} else {
			copyBitset(dividend, remainder, 8);
		}
	}
	delete[] dividend;
	delete[] divisor;
	return quotient;
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
	printBitset(r4, 16);
	delete[] r4;

	cout << "\n\n[DIVIDE]";
	cout << "\n[/] b1 / b2 = ";
	char *remainder = new char[8]{};
	char *r6 = div(b1, b2, remainder);
	if (r6 != nullptr) {
		printBitset(r6);
		delete[] r6;
		cout << "\n[%] b1 % b2 = ";
		printBitset(remainder);
	}
	delete[] remainder;
	delete[] b1;
	delete[] b2;
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

	printTest(n1, n2);
	cout << "\n";
	char *a, *b;
	char *d, *m;
	for (int i = -128; i < 128; i++) {
		for (int j = -128; j < 128; j++) {
			if (j == 0) continue;
			a = toBitset((char) i);
			b = toBitset((char) j);
			m = new char[8];
			d = div(a, b, m);
			if (d == nullptr) {
				cout << i << " N " << j << "\n";
				continue;
			}
			if (to8BitNum(d) != i / j || to8BitNum(m) != i % j) {
				cout << i << " : " << j << "\n";
			}
			delete[] d;
			delete[] m;
			delete[] a;
			delete[] b;
		}
	}
	return 0;
}

#pragma clang diagnostic pop