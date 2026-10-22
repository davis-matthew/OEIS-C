/*
 * A366978  a(n) = Sum_{j=1..n} binomial(floor(n/j)+n,n+1).
 * https://oeis.org/A366978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} binomial(j+n-1, n) * floor(n/j)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard for safety */

    seq_t sum = 0;
    seq_t binom = 1;               /* binomial for j = 1 : binom(n, n) = 1 */

    for (idx_t j = 1; j <= n; ++j) {
        /* floor(n/j) as seq_t */
        seq_t flo = (seq_t)(n / j);
        sum += binom * flo;

        /* update binomial for next j, except after the last iteration */
        if (j == n) break;
        seq_t numerator   = (seq_t)(j + n);   /* j + n  */
        seq_t denominator = (seq_t)j;         /* j      */
        binom = (binom * numerator) / denominator;
    }

    return sum;
}
