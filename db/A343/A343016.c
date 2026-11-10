/*
 * A343016  a(n) is the least nonnegative m such that m*n + A001414(n) is not
 *          prime.
 * https://oeis.org/A343016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 0 such that m*n + A001414(n) is not prime
 *            (search over m; A001414 = sopfr)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9891 (2026-11-10)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static uint64_t A343016_sopfr(uint64_t n)
{
	uint64_t sum = 0;
	uint64_t t = n;
	while (t % 2 == 0 && t > 1) {
		sum += 2;
		t /= 2;
	}
	for (uint64_t p = 3; p * p <= t; p += 2) {
		while (t % p == 0) {
			sum += p;
			t /= p;
		}
	}
	if (t > 1) sum += t;
	return sum;
}

seq_t a(idx_t n)
{
	uint64_t s = A343016_sopfr((uint64_t)n);
	seq_t S = (seq_t)s;
	seq_t N = (seq_t)n;
	seq_t m = 0;
	for (;;) {
		seq_t v = m * N + S;
		if (v < 2 || !is_prime_seq(v)) return m;
		m++;
	}
}
