/*
 * A339327  a(n) = (A001414(prime(n)-1)*A001414(prime(n)+1)) mod prime(n).
 * https://oeis.org/A339327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (A001414(prime(n)-1)*A001414(prime(n)+1)) mod prime(n),
 *            where A001414 is the sum of prime factors with repetition
 * Range:     u8 n <= 61, u16 n <= 6660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9676 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A339327_pr = 0;
static size_t A339327_cnt = 0;

static void A339327_build(idx_t need)
{
	size_t lim = 128;
	for (;;) {
		char *s = (char *)calloc(lim + 1, 1);
		size_t c = 0, i, j;
		for (i = 2; i <= lim; i++) {
			if (!s[i]) {
				c++;
				for (j = i * i; j <= lim; j += i) s[j] = 1;
			}
		}
		if (c >= need) {
			uint64_t *arr = (uint64_t *)malloc(c * sizeof(uint64_t));
			size_t k = 0;
			for (i = 2; i <= lim; i++) if (!s[i]) arr[k++] = i;
			free(s);
			if (A339327_pr) free(A339327_pr);
			A339327_pr = arr;
			A339327_cnt = c;
			return;
		}
		free(s);
		lim *= 2;
	}
}

static uint64_t A339327_sopfr(uint64_t n)
{
	uint64_t r = 0, p;
	if (n < 2) return 0;
	while (n % 2 == 0) { n /= 2; r += 2; }
	for (p = 3; p * p <= n; p += 2) {
		while (n % p == 0) { n /= p; r += p; }
	}
	if (n > 1) r += n;
	return r;
}

seq_t a(idx_t n)
{
	uint64_t p, x, y;
	if (n > A339327_cnt || !A339327_pr) A339327_build(n);
	p = A339327_pr[n - 1];
	x = A339327_sopfr(p - 1) % p;
	y = A339327_sopfr(p + 1) % p;
	return (seq_t)((x * y) % p);
}
