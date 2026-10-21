/*
 * A249891  G.f.: Sum_{n>=0} x^n / (1+x)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          x^k] * [Sum_{k=0..n} C(n,k)^2 * (-x)^k].
 * https://oeis.org/A249891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * C(2*k, k) * C(n+k, n-k)^2
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 22, i128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binom(2*k, k);                   /* C(2k, k) */
        seq_t c    = binom(n + k, n - k);             /* C(n+k, n-k) */
        term *= c;
        term *= c;                                    /* square */
        if (k & 1)                                   /* (-1)^k */
            term = -term;
        sum += term;
    }
    return sum;
}
