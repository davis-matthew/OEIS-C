/*
 * A384366  a(n) = Sum_{k=0..n} 2^k * binomial(4*n+1,k) * binomial(4*n-k,n-k).
 * https://oeis.org/A384366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(3*n + k, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;         /* C(3*n + 0, 0) */
    seq_t p3 = 1;            /* 3^k */
    seq_t p2 = pow2(n);      /* 2^(n-k) when k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binom * 3^k * 2^(n-k) */
        seq_t term = binom;
        term = term * p3;
        term = term * p2;
        sum += term;

        if (k == n) break;

        /* Prepare for next k */
        /* binom_{k+1} = binom_k * (3*n + k + 1) / (k + 1) */
        seq_t num = binom * (3 * n + k + 1);
        binom = num / (k + 1);

        p3 *= 3;
        p2 /= 2;   /* exact division by 2 */
    }

    return sum;
}
