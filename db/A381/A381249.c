/*
 * A381249  Indices of records in k/A001414(k), k>=2.
 * https://oeis.org/A381249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k>=2 with k/A001414(k) > j/A001414(j) for all 2<=j<k
 *            (sieve search for records)
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A381249_rec = 0;
static size_t A381249_cnt = 0;
static size_t A381249_cap = 0;

static void A381249_build(uint64_t limit)
{
	uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
	uint32_t *sp = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
	uint64_t i, j;
	uint64_t bk = 0, bs = 1;
	if (!spf || !sp) { free(spf); free(sp); return; }
	for (i = 2; i <= limit; i++) {
		if (spf[i] == 0) {
			for (j = i; j <= limit; j += i)
				if (spf[j] == 0) spf[j] = (uint32_t)i;
		}
	}
	A381249_cnt = 0;
	for (i = 2; i <= limit; i++) {
		sp[i] = sp[i / spf[i]] + spf[i];
		if (bk == 0 || (unsigned __int128)i * bs > (unsigned __int128)bk * sp[i]) {
			bk = i; bs = sp[i];
			if (A381249_cnt == A381249_cap) {
				A381249_cap = A381249_cap ? A381249_cap * 2 : 256;
				A381249_rec = (uint64_t *)realloc(A381249_rec, A381249_cap * sizeof(uint64_t));
			}
			A381249_rec[A381249_cnt++] = i;
		}
	}
	free(spf);
	free(sp);
}

seq_t a(idx_t n)
{
	static uint64_t limit = 0;
	if (limit == 0) { limit = 1024; A381249_build(limit); }
	while ((idx_t)A381249_cnt < n && limit < ((uint64_t)1 << 31)) {
		limit *= 4;
		A381249_build(limit);
	}
	if (n < 1 || (idx_t)A381249_cnt < n) return 0;
	return (seq_t)A381249_rec[n - 1];
}
