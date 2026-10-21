/*
 * A290754  Number of 4 X n binary matrices that are "primitive"; that is,
 *          they cannot be expressed as a "tiling" by a smaller matrix.
 * https://oeis.org/A290754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * ( 2^{4n/d} - 2^{2n/d} ) where μ is the
 *            Möbius function. This counts the number of 4×n binary matrices
 *            that are not tiled by a smaller matrix
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

/* Compute the Möbius function μ(n).  Result is 1, 0, or −1. */
static int mobius(idx_t n) {
    if (n == 1) return 1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > 1) return 0;          /* squared prime factor */
    }
    return (cnt % 2 == 0) ? 1 : -1;      /* (−1)^{#primes} */
}

seq_t a(idx_t n) {
    seq_t pos = 0;                      /* sum of positive μ(d) terms */
    seq_t neg = 0;                      /* sum of negative μ(d) terms */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* First divisor d */
            idx_t d1 = d;
            int mu1 = mobius(d1);
            if (mu1 != 0) {
                idx_t exp1 = 4 * (n / d1);
                idx_t exp2 = 2 * (n / d1);
                seq_t diff = pow2(exp1) - pow2(exp2);
                if (mu1 > 0) pos += diff; else neg += diff;
            }

            /* Complementary divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d1) {
                int mu2 = mobius(d2);
                if (mu2 != 0) {
                    idx_t exp1 = 4 * (n / d2);
                    idx_t exp2 = 2 * (n / d2);
                    seq_t diff = pow2(exp1) - pow2(exp2);
                    if (mu2 > 0) pos += diff; else neg += diff;
                }
            }
        }
    }
    return pos - neg;                    /* pos ≥ neg for all n ≥ 1 */
}
