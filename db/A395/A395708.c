/*
 * A395708  Expansion of g^4*(4*g-3)/(3-2*g)^4, where g = 1+x*g^3 is the g.f.
 *          of A001764.
 * https://oeis.org/A395708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Sum_{k=0..n} 2^(n-k) * binomial(3*n+3, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* precompute 2^n */
    seq_t p = pow2(n);            /* p = 2^n */
    seq_t binom = 1;              /* binomial(3*n+3, 0) = 1 */
    seq_t sum   = 0;
    idx_t N = 3*n + 3;

    for (idx_t k = 0; k <= n; ++k) {
        /* accumulate the term 2^(n-k) * binomial(...) */
        sum += p * binom;

        if (k < n) {              /* update for next iteration */
            binom = binom * ((seq_t)(N - k));
            binom = binom / ((seq_t)(k + 1));

            /* 2^(n-(k+1)) = 2^(n-k) / 2   */
            p = p / 2;
        }
    }

    seq_t result = ((seq_t)(n + 1)) * sum;
    return result;
}
