/*
 * A388050  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(4*n+1,k).
 * https://oeis.org/A388050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(4*n+1,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    idx_t k;
    idx_t m = 4*n + 1;

    seq_t sum = 0;
    seq_t B = 1;     /* binomial(n,0)   */
    seq_t C = 1;     /* binomial(m,0)   */
    seq_t p3 = 1;    /* 3^0 */

    for (k = 0; k <= n; ++k) {
        seq_t term = p3 * B * C;
        sum += term;

        if (k == n) break;   /* last iteration, nothing to update */

        /* update binomials and power for next k */
        seq_t mult_n = (seq_t)(n - k);
        seq_t mult_m = (seq_t)(m - k);
        seq_t denom  = (seq_t)(k + 1);

        B = (B * mult_n) / denom;
        C = (C * mult_m) / denom;
        p3 = p3 * 3;
    }

    return sum;
}
