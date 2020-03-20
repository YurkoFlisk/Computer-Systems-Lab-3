#include <iostream>
#include <random>

using namespace std;

int main()
{
	int testCases, nDigits, nMaxDigits, x, m, includeBasic;
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
	uniform_int_distribution<> nDist(1, nMaxDigits), mDist(1, 1000000000), charDist(0, 9);
	mt19937 eng;
	for (int i = 0; i < testCases; ++i)
	{
		nDigits = nDist(eng);
		m = mDist(eng);
		uniform_int_distribution<> xDist(0, m - 1);
		x = xDist(eng);
		for (int j = 0; j < nDigits; ++j)
			cout << charDist(eng);
		cout << ' ' << x << ' ' << m << endl;
	}
	return 0;
}