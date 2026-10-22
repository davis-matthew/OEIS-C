/*
 * A368488  a(n) = Sum_{k=0..n} n^k * binomial(k+n-1,k).
 * https://oeis.org/A368488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(k+n-1, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;
    seq_t B   = 1;

    for (idx_t k = 0; k <= n; ++k) {
        sum += pow * B;
        if (k == n) break;                /* last iteration reached */

        pow = pow * (seq_t)n;              /* n^k+1 */
        B   = B * (seq_t)(k + n) / (seq_t)(k + 1);  /* next binomial */
    }

    return sum;
}
