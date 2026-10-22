/*
 * A385670  a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * binomial(3*n+1,k) *
 *          binomial(3*n-k,n-k).
 * https://oeis.org/A385670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(2*n+k, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    seq_t sum  = 0;
    seq_t pow2 = pow_uint(2, n); /* 2^n */
    seq_t pow5 = 1;             /* 5^0 */

    /* binomial(2n, 0) = 1 */
    seq_t binom = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * pow5;   /* (5^k * binom) */
        term *= pow2;               /* * 2^(n-k) */
        sum += term;

        if (k == n) break;          /* last term done */

        /* next binomial: binom(2n+(k+1), k+1) */
        binom = binom * (seq_t)(2 * n + k + 1) / (k + 1);

        /* update powers of 5 and 2 for the next k */
        pow5 *= 5;
        pow2 /= 2;
    }

    return sum;
}
