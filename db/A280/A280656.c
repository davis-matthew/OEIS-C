/*
 * A280656  Denominator of (Sum_{k=0..n^2-1}
 *          (-1)^k*sqrt(Pi)/(Gamma(1/2-k)*Gamma(1+k))) - n.
 * https://oeis.org/A280656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^{2*(n^2-1) - popcount(n^2-1)}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"


seq_t a(idx_t n)
{
    /* n^2 fits in uint64_t for all n considered in tests */
    idx_t sq  = n * n;
    idx_t m   = sq - 1;          /* n^2 - 1 */

    /* popcount of m (number of set bits) */
    idx_t pc = 0;
    uint64_t t = (uint64_t)m;
    while (t) {
        pc++;
        t &= t - 1;              /* clear the lowest set bit */
    }

    /* exponent of 2 for a(n) */
    idx_t exp = (m << 1) - pc;   /* 2*m - popcount(m) */

    return pow2(exp);            /* 2^exp as seq_t */
}
