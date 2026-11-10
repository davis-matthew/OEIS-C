/*
 * A308485  a(n) is the sum of the integer logs of all integers strictly
 *          between prime(n) and prime(n+1).
 * https://oeis.org/A308485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{prime(n) < j < prime(n+1)} A001414(j), computed with
 *            a smallest-prime-factor sieve
 * Range:     u8 n <= 29, u16 n <= 2145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9422 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
	uint64_t limit = 100;
	if (n > 6) {
		double dn = (double)n;
		limit = (uint64_t)(dn * (log(dn) + log(log(dn)))) + 100;
	}
	for (;;) {
		uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
		if (!spf) return 0;
		uint64_t i, j;
		for (i = 2; i <= limit; i++) {
			if (spf[i] == 0) {
				spf[i] = (uint32_t)i;
				for (j = i * i; j <= limit; j += i)
					if (spf[j] == 0) spf[j] = (uint32_t)i;
			}
		}
		uint64_t cnt = 0, p = 0, q = 0;
		for (i = 2; i <= limit; i++) {
			if (spf[i] == i) {
				cnt++;
				if (cnt == (uint64_t)n) p = i;
				if (cnt == (uint64_t)n + 1) { q = i; break; }
			}
		}
		if (q != 0) {
			seq_t s = 0;
			for (j = p + 1; j < q; j++) {
				uint64_t x = j;
				while (x > 1) {
					uint64_t f = spf[x];
					s += (seq_t)f;
					x /= f;
				}
			}
			free(spf);
			return s;
		}
		free(spf);
		limit *= 2;
	}
}
