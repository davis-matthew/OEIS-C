/*
 * A278145  Denominator of partial sums of the m=1 member of an m-family of
 *          series considered by Hardy with value 4/Pi (see A088538).
 * https://oeis.org/A278145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2^{max_{0<=k<=n} (4k - 2*popcnt(k) + v2(k+1))} where
 *            popcnt(k) is the number of 1-bits in k and v2(k+1) is the
 *            exponent of 2 dividing (k+1). This follows from the reduction of
 *            each term of the series r(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t max_exp = 0;          /* exponent of 2 in the denominator */

    for (idx_t k = 0; k <= n; ++k) {
        /* popcount of k */
        unsigned int pop = __builtin_popcountll((uint64_t)k);

        /* v2 of k+1: number of trailing zeros in binary representation */
        unsigned int v2 = __builtin_ctzll((uint64_t)(k + 1));

        /* exponent for this term: 4k - 2*popcnt(k) + v2(k+1) */
        idx_t e = 4 * k - 2 * pop + v2;

        if (e > max_exp) {
            max_exp = e;
        }
    }

    return pow2(max_exp);      /* 2^max_exp as sequence value */
}
