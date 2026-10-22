/*
 * A388047  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(n+1,k).
 * https://oeis.org/A388047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(n+1,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;                 /* accumulated sum */
    seq_t c1   = 1;                 /* C(n,0) */
    seq_t c2   = 1;                 /* C(n+1,0) */
    seq_t pow3 = 1;                 /* 3^0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = 3^k * C(n,k) * C(n+1,k) */
        seq_t term = pow3;
        term = term * c1;
        term = term * c2;
        sum = sum + term;

        if (k == n) break;          /* no more terms */

        /* update binomial coefficients for next k */
        c1 = c1 * (seq_t)(n - k) / (seq_t)(k + 1);          /* C(n,k+1) */
        c2 = c2 * (seq_t)(n + 1 - k) / (seq_t)(k + 1);      /* C(n+1,k+1) */
        pow3 = pow3 * 3;                          /* 3^(k+1) */
    }

    return sum;
}
