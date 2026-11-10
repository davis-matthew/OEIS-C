/*
 * A317743  Valleys in A064800: terms which are smaller than their neighbors.
 * https://oeis.org/A317743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k + bigomega(k) for the n-th k >= 2 with bigomega(k-1) >=
 *            bigomega(k)+2 and bigomega(k+1) >= bigomega(k) (i.e. the n-th
 *            local minimum of A064800(k) = k + bigomega(k)); found by sieve
 *            scan
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint8_t *A317743_om = 0;
static uint64_t A317743_lim = 0;

static void A317743_build(uint64_t lim)
{
    uint32_t *spf = (uint32_t *)malloc((lim + 1) * sizeof(uint32_t));
    uint8_t *om = (uint8_t *)malloc(lim + 1);
    uint64_t i, j;
    for (i = 0; i <= lim; i++) spf[i] = 0;
    for (i = 2; i <= lim; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= lim; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    om[0] = 0;
    if (lim >= 1) om[1] = 0;
    for (i = 2; i <= lim; i++) om[i] = (uint8_t)(om[i / spf[i]] + 1);
    free(spf);
    if (A317743_om) free(A317743_om);
    A317743_om = om;
    A317743_lim = lim;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A317743_lim == 0) A317743_build(1u << 16);
    for (;;) {
        uint64_t cnt = 0, k;
        for (k = 2; k + 1 <= A317743_lim; k++) {
            unsigned w = A317743_om[k];
            if ((unsigned)A317743_om[k - 1] >= w + 2 && (unsigned)A317743_om[k + 1] >= w) {
                cnt++;
                if (cnt == (uint64_t)n) return (seq_t)k + (seq_t)w;
            }
        }
        A317743_build(A317743_lim * 2);
    }
}
