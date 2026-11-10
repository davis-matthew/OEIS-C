/*
 * A393611  a(n) = least positive integer k such that A002476(n) + 3^k + 1 is
 *          prime, or -1 if no such prime exists.
 * https://oeis.org/A393611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>=1 such that A002476(n) + 3^k + 1 is prime
 *            (A002476 = primes 6m+1); found by search, -1 if none
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A393611_u128;

static uint64_t A393611_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
	return (uint64_t)(((A393611_u128)a * b) % m);
}

static uint64_t A393611_powmod(uint64_t b, uint64_t e, uint64_t m)
{
	uint64_t r = 1;
	b %= m;
	while (e) {
		if (e & 1) r = A393611_mulmod(r, b, m);
		b = A393611_mulmod(b, b, m);
		e >>= 1;
	}
	return r;
}

static bool A393611_isprime(uint64_t n)
{
	static const uint64_t sp[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
	if (n < 2) return false;
	for (int i = 0; i < 12; i++) {
		if (n == sp[i]) return true;
		if (n % sp[i] == 0) return false;
	}
	uint64_t d = n - 1;
	int s = 0;
	while ((d & 1) == 0) { d >>= 1; s++; }
	for (int i = 0; i < 12; i++) {
		uint64_t x = A393611_powmod(sp[i], d, n);
		if (x == 1 || x == n - 1) continue;
		bool comp = true;
		for (int r = 1; r < s; r++) {
			x = A393611_mulmod(x, x, n);
			if (x == n - 1) { comp = false; break; }
		}
		if (comp) return false;
	}
	return true;
}

seq_t a(idx_t n)
{
	uint64_t p = 1;
	idx_t cnt = 0;
	while (cnt < n) {
		p += 6;
		if (A393611_isprime(p)) cnt++;
	}
	uint64_t pw = 1;
	for (int k = 1; k <= 38; k++) {
		pw *= 3;
		uint64_t v = p + pw + 1;
		if (A393611_isprime(v)) return (seq_t)k;
	}
	return (seq_t)0 - (seq_t)1;
}
