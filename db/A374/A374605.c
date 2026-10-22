/*
 * A374605  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k,
 *          k)*binomial(3*n+2*k, n).
 * https://oeis.org/A374605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n}
 *            binomial(n,k)^2*binomial(n+k,k)*binomial(3*n+2*k,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t b1 = binom(n, k);                     /* binomial(n,k)           */
        seq_t b2 = binom(n + k, k);                 /* binomial(n+k,k)         */
        seq_t b3 = binom(3 * n + 2 * k, n);         /* binomial(3n+2k,n)       */
        seq_t term = b1 * b1;                       /* (binomial(n,k))^2       */
        term *= b2;                                 /* * binomial(n+k,k)       */
        term *= b3;                                 /* * binomial(3n+2k,n)     */
        total += term;                              /* accumulate sum          */
    }
    return total;
}
