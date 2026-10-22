/*
 * A366116  Expansion of (1/x) * Series_Reversion( x*(1+x+x^3)/(1+x)^2 ).
 * https://oeis.org/A366116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} (-1)^k * *
 *            binomial(n+k,k) * binomial(n-k+1,n-3k)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 24, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* kmax = floor(n/3) */
    idx_t kmax = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* binomial(n+k,k) * binomial(n-k+1,n-3*k) */
        seq_t t1 = binomial(n + k, k);
        seq_t t2 = binomial(n - k + 1, n - 3 * k);
        seq_t prod = t1 * t2;

        if (k & 1)  /* odd k -> negative sign */
            sum -= prod;
        else
            sum += prod;
    }

    /* divide by (n+1); (n+1) fits in seq_t as an unsigned value */
    return sum / ((seq_t)n + 1);
}
