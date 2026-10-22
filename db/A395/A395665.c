/*
 * A395665  Expansion of g^6/(5-4*g)^4, where g = 1+x*g^5 is the g.f. of
 *          A002294.
 * https://oeis.org/A395665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(k+2,2) * binomial(5*n+5, n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* N = 5*n + 5 */
    idx_t N = 5 * n + 5;

    /* Compute C(N, n) iteratively */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* binom = binom * (N - i + 1) / i */
        binom = binom * (N - i + 1) / i;
    }

    seq_t sum   = (seq_t)0;
    seq_t pow4  = (seq_t)1;          /* 4^k, starts at k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* small = binomial(k+2, 2) = (k+2)(k+1)/2 */
        seq_t small = ((k + 2) * (k + 1)) / 2u;

        /* term = 4^k * small * binom */
        seq_t term = pow4 * small * binom;
        sum += term;

        /* If this was the last term, exit loop */
        if (k == n) {
            break;
        }

        /* Update for next k
         * 1. Increase 4^k to 4^(k+1)
         * 2. Decrease r = n - k by 1 in binom:
         *    binom_{next} = binom * r / (N - r + 1)
         */
        pow4 *= (seq_t)4;

        idx_t r = n - k;                 /* current exponent of binomial(r) */
        binom = binom * r / (N - r + 1);
    }

    return sum;
}
