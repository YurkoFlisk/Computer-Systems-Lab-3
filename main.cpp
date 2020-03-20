#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

using namespace std;

// Struct for output of primeFactor function. Represents prime
// factor of the input number together with its power.
// I.e., the struct holds a single (p^e) part of prime factorization.
struct PrimeFactor
{
	PrimeFactor(int64_t prime, int64_t power)
		: prime(prime), power(power)
	{}
	int64_t prime;
	int64_t power;
};

// Struct for input to solveCongruences function. Specifies that
// ('x' = 'rem' mod 'factor'), where 'x' is the solution
// to be returned. I.e., the struct holds a single congruency
struct Congruency
{
	Congruency(int64_t factor, int64_t rem)
		: factor(factor), rem(rem)
	{}
	int64_t factor;
	int64_t rem;
};

using PrimeFactorization = std::vector<PrimeFactor>;
using CongruencyList = std::vector<Congruency>;
template<typename Key, typename Value>
using Map = unordered_map<Key, Value>;
constexpr int64_t P = 998244353;

// Trivial prime factorization method, O(sqrt(n))
PrimeFactorization primeFactor(int64_t n)
{
	PrimeFactorization primePowers;
	for (int64_t d = 2; d * d <= n; ++d)
	{
		int64_t cnt = 0;
		while (true)
		{
			const int64_t q = n / d;
			if (n - q * d)
				break;
			n = q;
			++cnt;
		}
		if (cnt)
			primePowers.emplace_back(d, cnt);
	}
	if (n != 1)
		primePowers.emplace_back(n, 1);
	return primePowers;
}

// Euler totient function
// Assumes 'primes' is a valid prime factorization of 'n'
int64_t phi(int64_t n, const PrimeFactorization& primes)
{
	for (const auto& primePower : primes)
		n -= n / primePower.prime;
	return n;
}

// Power function
// O(p), so assumes small values of p
int64_t power(int64_t base, int64_t p)
{
	int64_t ans = 1;
	while (p--)
		ans *= base;
	return ans;
}

// Computes base^p mod m in O(log p) time
int64_t powmod(int64_t base, int64_t p, int64_t m)
{
	int64_t ret = 1;
	for (; p; p >>= 1, base = base * base % m)
		if (p & 1)
			ret = ret * base % m;
	return ret;
}

// Extended Euclidean algorithm
int64_t gcd(int64_t first, int64_t second, int64_t& coef_first, int64_t& coef_second)
{
	if (first == 0)
	{
		coef_first = 0, coef_second = 1;
		return second;
	}
	int64_t cf1, cs1;
	const int64_t g = gcd(second % first, first, cf1, cs1);
	coef_first = cs1 - (second / first) * cf1;
	coef_second = cf1;
	return g;
}

// Solves congruences by Chinese remainder theorem
// Assumes 'n' is factorized to pairwise coprime 'factors' of 'congruences'
int64_t solveCongruences(int64_t n, const CongruencyList& congruences)
{
	int64_t ans = 0, m, cm, cfactor;
	for (const auto& cong : congruences)
	{
		m = n / cong.factor;
		gcd(m, cong.factor, cm, cfactor);
		if (cm < 0)
			cm += cong.factor;
		ans = (ans + m * cm * cong.rem) % n;
	}
	return ans;
}

// Computes multiplicative order of 'base' modulo 'm', assuming they're coprime
// 'gPrimes' is factorization of multiplicative group size (phi(m))
// 'primeI' and 'divisor' are used internally by recursive implementation
// If no such order found, -1 is returned.
int64_t order(int64_t base, int64_t m, const PrimeFactorization& gPrimes,
	const size_t primeI = 0, int64_t divisor = 1)
{
	const auto& curPrime = gPrimes[primeI];
	int64_t ord;
	if (primeI + 1 == gPrimes.size())
	{
		for (int64_t i = 0; i <= curPrime.power; ++i, divisor *= curPrime.prime)
			if (powmod(base, divisor, m) == 1)
				return divisor;
	}
	else
		for (int64_t i = 0; i <= curPrime.power; ++i, divisor *= curPrime.prime)
			if ((ord = order(base, m, gPrimes, primeI + 1, divisor)) != -1)
				return ord;
	return -1;
}

// Baby-step giant-step algorithm for discrete logarithm (O(sqrt(groupSize)))
// binv is b^(-1) mod m
bool babyStepGiantStep(int64_t base, int64_t binv, int64_t x,
	int64_t m, int64_t groupSize, int64_t& dl)
{
	const int64_t n = ceil(sqrt(groupSize));
	Map<int64_t, int64_t> babySteps;
	for (int64_t j = 0; j < n; ++j)
		babySteps.insert({ powmod(base, j, m), j }); // Only smallest j will get stored
	for (int64_t i = 0; i <= n; ++i)
	{
		auto it = babySteps.find((x * powmod(binv, (int64_t)(n)*i, m)) % m);
		if (it != babySteps.end())
		{
			dl = it->second + (int64_t)(n)*i;
			return true;
		}
	}
	return false; // Should not happen
}

// Pohlig-Hellman algorithm
// Computes (dl mod (p^pPower)) where (base^dl = x mod m),
// and groupSize is size of group modulo m, i.e. phi(m),
// and p^Power is divisor of groupSize
// binv is b^(-1) mod m
bool pohligHellman(int64_t base, int64_t binv, int64_t x,
	int64_t m, int64_t p, int64_t pPower, int64_t groupSize, int64_t& dl)
{
	int64_t ans = 0, ansJ;
	int64_t pInJ, pInJp1 = 1; // on each iteration is p^j and p^(j + 1) respectively
	const int64_t base1 = powmod(base, groupSize / p, m);
	const int64_t binv1 = powmod(binv, groupSize / p, m);
	for (int64_t j = 0; j < pPower; ++j)
	{
		pInJ = pInJp1;
		pInJp1 *= p;
		
		if (!babyStepGiantStep(base1, binv1, powmod(x, groupSize / pInJp1, m), m, p, ansJ))
			return false;
		x = (x * powmod(binv, pInJ * ansJ, m)) % m;
		ans += pInJ * ansJ;
	}
	dl = ans;
	return true;
}

// Slower but correct (accepted for 45 points on INVXOR
// problem on CodeChef) algorithm for testing purposes
//bool discLogRight(int64_t base, int64_t x, int64_t m, int64_t& dl)
//{
//	if (x == 1 || m == 1)
//	{
//		dl = 0;
//		return true;
//	}
//
//	int64_t binv, b1inv, temp, temp2;
//	const int64_t g = gcd(base, m, binv, temp);
//	if (g != 1) // if base and m are not coprime
//	{
//		if (x % g)
//			return false;
//		const int64_t base1 = base / g, m1 = m / g, x1 = x / g;
//		gcd(base1, m1, b1inv, temp);
//		b1inv += m; // binv should be non-negative
//		const bool success = discLogRight(base % m1, (x1 * b1inv) % m1, m1, dl);
//		++dl;
//		return success;
//	}
//	if (gcd(x, m, temp, temp2) != 1) // if x and m aren't coprime (when base and m are) 
//		return false;
//	if (binv < 0)
//		binv += m; // Should be non-negative
//
//	const PrimeFactorization primesM = primeFactor(m);
//	const int64_t groupSize = phi(m, primesM);
//	return babyStepGiantStep(base, binv, x, m, groupSize, dl);
//}

// Discrete logarithm of 'x' modulo 'm' with base 'base'
// Returns whether it exists, and if yes, writes it to 'dl'
// For case of coprime 'base' and 'm' uses Pohlig-Hellman algorithm
bool discLog(int64_t base, int64_t x, int64_t m, int64_t& dl)
{
	if (x == 1 || m == 1)
	{
		dl = 0;
		return true;
	}

	int64_t binv, b1inv, temp, temp2, dl_mod_pInPow;
	const int64_t g = gcd(base, m, binv, temp);
	if (g != 1) // if base and m are not coprime
	{
		if (x % g)
			return false;
		const int64_t base1 = base / g, m1 = m / g, x1 = x / g;
		gcd(base1, m1, b1inv, temp);
		if (b1inv < 0)
			b1inv += m; // binv should be non-negative
		const bool success = discLog(base % m1, (x1 * b1inv) % m1, m1, dl);
		++dl;
		return success;
	}
	if (gcd(x, m, temp, temp2) != 1) // if x and m aren't coprime (when base and m are) 
		return false;
	if (binv < 0)
		binv += m; // Should be non-negative

	// Compute multiplicative group parameters
	const PrimeFactorization primesM = primeFactor(m);
	const int64_t groupSize = phi(m, primesM);
	PrimeFactorization primesG = primeFactor(groupSize);
	const int64_t subgroupSize = order(base, m, primesG);
	if (subgroupSize == 1)
	{
		dl = base;
		return base == x; // if false, 'dl' shall be unused
	}
	// If base is generator of the group (its subgroup (size) is the whole group (size)),
	// we don't even need to calculate factorization of subgroup order, because it's already computed 
	const PrimeFactorization primesSG = (subgroupSize == groupSize ?
		std::move(primesG) : primeFactor(subgroupSize));
	// Pohlig-Hellman algorithm
	CongruencyList congruences;
	for (const auto& factor : primesSG)
	{
		const int64_t p = factor.prime, pPower = factor.power;
		const int64_t pInPow = power(p, pPower);
		if (!pohligHellman(base, binv, x, m, p, pPower, subgroupSize, dl_mod_pInPow))
			return false;
		congruences.emplace_back(pInPow, dl_mod_pInPow);
	}
	dl = solveCongruences(subgroupSize, congruences);
	return true;
}

// Computes (base^exp mod m) where 'exp' is 'd10_cnt'-digit decimal
// representation of the exponent. Pointer is to the least significant digit
int64_t powmod10(int64_t base, char* exp, int64_t d10_cnt, int64_t m)
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

//void test()
//{
//	bool succRight, succ;
//	int64_t dlRight, dl;
//	succRight = discLogRight(0, 2, 4, dlRight);
//	succ = discLog(0, 2, 4, dl);
//	for (int64_t m = 1; m < 10; ++m)
//		for (int64_t x = 0; x < m; ++x)
//			for (int64_t base = 0; base < 10; ++base)
//				if ((succRight = discLogRight(base, x, m, dlRight))
//					!= (succ = discLog(base, x, m, dl)) || (succ && dlRight != dl))
//				{
//					cout << base << ' ' << x << ' ' << m << ". Right: " << succRight
//						<< ' ' << dlRight << ", got: " << succ << ' ' << dl << endl;
//				}
//}

// The Main function
int main(void)
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	// test();
	int64_t t = 0, m, x;
	char nbuf[10002];
	cin >> t;
	while (t--)
	{
		cin >> nbuf >> x >> m;

		if (m == 1 || x == 1)
		{
			cout << "0\n";
			continue;
		}

		int64_t nModP1 = 0, pb, d10_cnt(0), exp2n;
		char* pnbuf = nbuf, ch;
		while ((ch = *pnbuf++) != '\0')
			nModP1 = (nModP1 * 10 + (ch - '0')) % (P - 1), ++d10_cnt;
		----pnbuf;
		if (x == 0 && (d10_cnt > 1 || *pnbuf > '1'))
		{
			cout << "1\n";
			continue;
		}

		exp2n = powmod10(2, pnbuf, d10_cnt, m);
		if (!discLog((exp2n + m - 1) % m, x, m, pb))
			cout << "-1\n";
		else
		{
			const int64_t ans = (powmod(2, (nModP1 + P - 2) % (P - 1), P)
				* (powmod(2, pb, P) - 1)) % P;
			cout << ans << '\n';
		}
	}
	return 0;
}