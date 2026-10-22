/*
 * A373682  Expansion of e.g.f. exp(x / (1 - x^2)^3) / (1 - x^2).
 * https://oeis.org/A373682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} binomial(3*n-5*k,k)/(n-2*k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binom_multiplicative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_multiplicative.h"
/* This implements the summation directly, using a multiplicative binomial
   routine and an incremental factorial ratio P = n!/(n-2k)! to avoid large
   intermediate factorials. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t P   = 1;                    /* P = n!/(n-2k)!   */
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binom_multiplicative(3 * n - 5 * k, k);
        sum += P * bin;

        if (k < maxk) {
            /* Update P to n!/(n-2(k+1))! = P * (n-2k-1)*(n-2k) */
            P *= (seq_t)(n - 2 * k - 1);
            P *= (seq_t)(n - 2 * k);
        }
    }
    return sum;
}
