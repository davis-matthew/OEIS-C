/*
 * A383884  a(n) = Sum_{k=0..n} 2^k * binomial(2*n+1,k) * binomial(2*n-k,n-k).
 * https://oeis.org/A383884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i;
    seq_t term = 1;                 /* term = 3^0 * 2^(n-0) * C(n+0,0) = 2^n at start */
    /* compute 2^n using repeated multiplication (overflow is irrelevant
       for large n; intermediate overflows are truncated in seq_t) */
    for (i = 0; i < n; ++i) {
        unsigned __int128 tmp = (unsigned __int128)term * 2;
        term = (seq_t)tmp;
    }

    seq_t sum = 0;
    for (i = 0; i <= n; ++i) {
        sum += term;               /* add current term to the sum */
        if (i == n) break;         /* last term reached */

        /* compute next term:
           term_{k+1} = term_k * 3 * (n + k + 1) / (2 * (k + 1))   */
        unsigned __int128 numerator = (unsigned __int128)term * 3 * (n + i + 1);
        idx_t denom = 2 * (i + 1);
        term = (seq_t)(numerator / denom);
    }
    return sum;
}
