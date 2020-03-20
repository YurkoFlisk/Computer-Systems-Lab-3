#include <iostream>
#include <string>
#include <random>

using namespace std;

// Computes base^p mod m in O(log p) time
int64_t powmod(int64_t base, int64_t p, int64_t m)
{
	int64_t ret = 1;
	for (; p; p >>= 1, base = base * base % m)
		if (p & 1)
			ret = ret * base % m;
	return ret;
}

// Computes (base^exp mod m) where 'exp' is 'd10_cnt'-digit decimal
// representation of the exponent. Pointer is to the least significant digit
int64_t powmod10(int64_t base, const char* exp, int64_t d10_cnt, int64_t m)
{
	int64_t ans = 1, base2, base4, base6, ansMul = 1;
	while (d10_cnt--)
	{
		const int curDigit = *exp-- - '0';
		base2 = base * base % m;
		base4 = base2 * base2 % m;
		base6 = base4 * base2 % m;

		// ans = (ans * powmod(base, curDigit, m)) % m;
		switch (curDigit)
		{
		case 0: ansMul = 1; break;
		case 1: ansMul = base; break;
		case 2: ansMul = base2; break;
		case 3: ansMul = base2 * base % m; break;
		case 4: ansMul = base4; break;
		case 5: ansMul = base4 * base % m; break;
		case 6: ansMul = base6; break;
		case 7: ansMul = base6 * base % m; break;
		case 8: ansMul = base6 * base2 % m; break;
		case 9: ansMul = base6 * base2 % m * base % m; break;
		}
		ans = (ans * ansMul) % m;

		// base = powmod(base, 10, m);
		base = (base6 * base4) % m;
	}
	return ans;
}

int main()
{
	int testCases, nMaxDigits, includeBasic;
	cin >> testCases >> nMaxDigits >> includeBasic;
	if (testCases < 0 || nMaxDigits < 1)
	{
		cerr << "Wrong input" << endl;
		return 0;
	}
	if (includeBasic) // Include basic cases found on official INVXOR problem page
	{
		cout << testCases + 3 << endl;
		cout << "2 2 7\n4 3 17\n3 5 13" << endl;
	}
	else
		cout << testCases << endl;
	uniform_int_distribution<> nDist(1, nMaxDigits), mDist(1, 1000000000), charDist('0', '9');
	random_device rnddev;
	mt19937 eng(rnddev());
	for (int i = 0; i < testCases; ++i)
	{
		const int nDigits = nDist(eng);
		const int64_t m = mDist(eng);
		uniform_int_distribution<> pbDist(0, m);
		const int64_t pb = pbDist(eng);
		string n(nDigits, 0);
		for (int j = 0; j < nDigits; ++j)
			n[j] = charDist(eng);
		const int64_t exp2n = powmod10(2, &n.back(), nDigits, m);
		const int64_t x = powmod((exp2n + m - 1) % m, pb, m);
		cout << n << ' ' << x << ' ' << m << endl;
	}
	return 0;
}