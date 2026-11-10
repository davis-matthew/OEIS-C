/*
 * A337028  Numbers k such that k + A001414(k), k - A001414(k) and k mod
 *          A001414(k) are all prime.
 * https://oeis.org/A337028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that k+A001414(k), k-A001414(k) and k mod
 *            A001414(k) are all prime; found by a smallest-prime-factor sieve
 *            search
 * Range:     u8 n <= 24, u16 n <= 655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9341 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A337028_terms = 0;
static size_t A337028_cnt = 0;

static void A337028_build(uint64_t L)
{
	uint64_t M = 2 * L + 2, i, j, k;
	uint32_t *spf = (uint32_t *)calloc((size_t)(M + 1), sizeof(uint32_t));
	size_t cap = 1024;
	uint64_t *t;
	if (!spf) return;
	for (i = 2; i <= M; i++) {
		if (spf[i] == 0) {
			for (j = i; j <= M; j += i)
				if (spf[j] == 0) spf[j] = (uint32_t)i;
		}
	}
	t = (uint64_t *)malloc(cap * sizeof(uint64_t));
	size_t c = 0;
	for (k = 4; k <= L; k++) {
		if (spf[k] == k) continue;
		uint64_t s = 0, m = k;
		while (m > 1) {
			uint64_t p = spf[m];
			s += p;
			m /= p;
		}
		if (s >= k) continue;
		uint64_t d = k - s, u = k + s, r = k % s;
		if (d < 2 || r < 2) continue;
		if (spf[d] != d || spf[u] != u || spf[r] != r) continue;
		if (c == cap) {
			cap *= 2;
			t = (uint64_t *)realloc(t, cap * sizeof(uint64_t));
		}
		t[c++] = k;
	}
	free(spf);
	free(A337028_terms);
	A337028_terms = t;
	A337028_cnt = c;
}

seq_t a(idx_t n)
{
	static uint64_t L = 0;
	while (A337028_cnt < (size_t)n) {
		L = L ? L * 2 : 4096;
		A337028_build(L);
	}
	return (seq_t)A337028_terms[n - 1];
}
