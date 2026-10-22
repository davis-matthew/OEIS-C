/*
 * A386767  a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(2*n+k-1,k).
 * https://oeis.org/A386767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 2^(n-k) * binomial(2*n+k-1,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External integer types are defined by the build environment */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* total sum */
    seq_t total = 0;

    /* binomial coefficient A_k = binomial(2*n+k-1, k).  A_0 = 1. */
    seq_t binom = 1;

    /* 5^k, 2^(n-k), both updated incrementally. */
    seq_t p5 = 1;  /* 5^0 */
    seq_t p2 = 1;  /* 2^n initially, will be divided by 2 each step */

    /* Compute p2 = 2^n */
    for (idx_t i = 0; i < n; ++i)
        p2 *= 2;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binom * 5^k * 2^(n-k) */
        seq_t term = binom * p5 * p2;
        total += term;

        /* prepare values for next iteration (unless last step) */
        if (k == n)
            break;

        /* update 5^k */
        p5 *= 5;

        /* update 2^(n-k) by dividing by 2 */
        p2 /= 2;

        /* update binomial coefficient for next k:
           A_{k+1} = A_k * (2*n + k) / (k+1) */
        seq_t mult = binom * (seq_t)(2*n + k);
        binom = mult / (seq_t)(k + 1);
    }

    return total;
}
