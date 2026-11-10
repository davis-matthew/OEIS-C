/*
 * A396784  Least positive integer k such that A001414(k+1) - A001414(k) = n.
 * https://oeis.org/A396784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k>=1 with A001414(k+1) - A001414(k) = n; found by
 *            linear search over k
 * Range:     u8 n <= 24, u16 n <= 959, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 958 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A001414_sopfr(idx_t n)
{
	if (n == 1) return 0;

	seq_t result = 0;
	idx_t temp = n;

	idx_t count = 0;
	while (temp % 2 == 0) {
		temp /= 2;
		count++;
	}
	result += (seq_t)2 * (seq_t)count;

	for (idx_t p = 3; temp / p >= p; p += 2) {
		count = 0;
		while (temp % p == 0) {
			temp /= p;
			count++;
		}
		result += (seq_t)p * (seq_t)count;
	}

	if (temp > 1) {
		result += (seq_t)temp;
	}

	return result;
}

seq_t a(idx_t n)
{
	seq_t target = (seq_t)n;
	idx_t k = 1;
	seq_t s0 = A001414_sopfr(1);
	for (;; k++) {
		seq_t s1 = A001414_sopfr(k + 1);
		if (s1 >= s0 && s1 - s0 == target) {
			return (seq_t)k;
		}
		s0 = s1;
	}
}
