/*
 * A302784  Inverse permutation to A302783: a(n) = A003188(A052331(n)).
 * https://oeis.org/A302784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003188(A052331(n)) = m XOR floor(m/2), where m =
 *            A052331(n)
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 102, u64 n <= 262,
 *            u128 n <= 640, big any n
 * Validated: standard, pass, 86 terms through n = 4040 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t gray_shift(unsigned long long bit)
{
	seq_t p = 1;
	for (unsigned long long i = 0; i < bit; ++i)
		p *= 2;
	return p;
}

seq_t a(idx_t n)
{
	if (n <= 1)
		return 0;
	size_t lim = (size_t)n;
	unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
	unsigned char *fd = (unsigned char *)calloc(lim + 1, 1);
	uint32_t *rank = (uint32_t *)calloc(lim + 1, sizeof(uint32_t));
	for (size_t i = 2; i * i <= lim; ++i)
		if (!comp[i])
			for (size_t j = i * i; j <= lim; j += i)
				comp[j] = 1;
	for (size_t p = 2; p <= lim; ++p) {
		if (comp[p])
			continue;
		uint64_t v = p;
		while (v <= lim) {
			fd[v] = 1;
			if (v > lim / v)
				break;
			v = v * v;
		}
	}
	uint32_t c = 0;
	for (size_t i = 0; i <= lim; ++i) {
		if (fd[i])
			++c;
		rank[i] = c;
	}

	seq_t m = 0;
	uint64_t t = (uint64_t)n;
	for (uint64_t p = 2; p * p <= t; ++p) {
		if (t % p == 0) {
			unsigned e = 0;
			while (t % p == 0) {
				t /= p;
				++e;
			}
			uint64_t v = p;
			while (e) {
				if (e & 1U)
					m += gray_shift(rank[v] - 1);
				e >>= 1;
				if (e)
					v = v * v;
			}
		}
	}
	if (t > 1)
		m += gray_shift(rank[t] - 1);

	free(comp);
	free(fd);
	free(rank);
	return m ^ (m >> 1);
}
