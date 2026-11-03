/*
 * A361944  a(n) is the least k > 0 such that the binary expansion of k*n is
 *          an abelian square (A272653).
 * https://oeis.org/A361944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) is the least k>0 such that the binary expansion of k*n * is
 *            an abelian square (i.e. splits into two halves with the same *
 *            number of ones). The function searches for the first such k
 * Range:     u8 n <= 15, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-01)
 * Helpers:   bit_length, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/pow2.h"

static inline int count_ones(seq_t x)
{
    int cnt = 0;
    while (x) {
        x &= x - 1;   /* erase lowest set bit */
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, but guard against zero */

    for (idx_t k = 1; ; ++k) {
        seq_t prod = (seq_t)n * (seq_t)k;
        unsigned L = bit_length(prod);
        if ((L & 1) == 0) {                 /* length must be even */
            unsigned m = L >> 1;            /* half length */
            seq_t mask = pow2(m) - 1;       /* lower m bits mask */
            seq_t lower = prod & mask;
            seq_t upper = prod >> m;        /* higher m bits */

            if (count_ones(lower) == count_ones(upper))
                return k;                    /* minimal k found */
        }
    }
}
