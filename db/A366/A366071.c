/*
 * A366071  Expansion of (1/x) * Series_Reversion( x*(1+x-x^3)/(1+x) ).
 * https://oeis.org/A366071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} (-1)^(n-k) * binomial(n+k,k) *
 *            binomial(n-2k-1, n-3k)
 * Range:     i8 n <= 9, i16 n <= 14, i32 n <= 28, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* Ensure the binomial arguments are non‑negative and admissible. */
        if (n < 2 * k + 1) continue;   /* n-2k-1 would be negative */
        if (n < 3 * k)      continue;   /* n-3k   would be negative */

        idx_t top    = n - 2 * k - 1;
        idx_t bottom = n - 3 * k;

        if (top < bottom) continue;     /* binomial would be zero */

        seq_t t1 = binom(n + k, k);
        seq_t t2 = binom(top, bottom);

        seq_t term = t1 * t2;
        if (((n - k) & 1) == 1) term = -term;

        sum += term;
    }

    seq_t denom = (seq_t)(n + 1);
    return sum / denom;
}
