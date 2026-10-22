/*
 * A394722  Number of transitive relations on an n-set with at most four
 *          ordered pairs.
 * https://oeis.org/A394722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,0) + C(n,1) + 10*C(n,2) + 91*C(n,3) + 503*C(n,4) +
 *            1630*C(n,5) + 3060*C(n,6) + 3360*C(n,7) + 1680*C(n,8)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 25, u64 n <= 382,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const int coeff[9] = {1, 1, 10, 91, 503, 1630, 3060, 3360, 1680};
    seq_t sum = 0;
    for (int k = 0; k < 9; ++k) {
        if (n < (idx_t)k) break;          /* binom(n,k)=0 for k>n */
        seq_t term = binom(n, k);
        sum += term * coeff[k];
    }
    return sum;
}
