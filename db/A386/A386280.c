/*
 * A386280  Numbers k > 1 such that A066328(k) / A001222(k) = x + 1/2 for some
 *          x >= 0.
 * https://oeis.org/A386280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k > 1 such that 2*A066328(k) / A001222(k) is an odd
 *            integer; found by sieve search (smallest prime factor sieve,
 *            doubling the limit)
 * Range:     u8 n <= 54, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A386280_terms = NULL;
static size_t A386280_count = 0;
static size_t A386280_limit = 0;

static void A386280_build(size_t L)
{
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *terms = (uint32_t *)malloc((L / 2 + 16) * sizeof(uint32_t));
    size_t cnt = 0;
    uint32_t pc = 0;
    size_t i, j, k;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            pidx[i] = ++pc;
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (k = 2; k <= L; k++) {
        size_t m = k;
        uint64_t s = 0, om = 0;
        uint32_t last = 0;
        while (m > 1) {
            uint32_t p = spf[m];
            m /= p;
            om++;
            if (p != last) { s += pidx[p]; last = p; }
        }
        if ((2 * s) % om == 0 && (((2 * s) / om) & 1)) {
            terms[cnt++] = (uint32_t)k;
        }
    }
    free(spf);
    free(pidx);
    free(A386280_terms);
    A386280_terms = terms;
    A386280_count = cnt;
    A386280_limit = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A386280_limit == 0) A386280_build(1 << 12);
    while (A386280_count < (size_t)n)
        A386280_build(A386280_limit * 2);
    return (seq_t)A386280_terms[n - 1];
}
