/*
 * A366051  Expansion of (1/x) * Series_Reversion( x/(1-x+x^3) ).
 * https://oeis.org/A366051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} (-1)^(n-k) *
 *            binomial(n+1,k) * binomial(n-k+1,n-3*k)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 25, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;            /* floor(n/3) */
    seq_t sum  = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial(n + 1, k);
        seq_t term2 = binomial(n - k + 1, n - 3 * k);
        seq_t term  = term1 * term2;

        /* apply the (-1)^(n-k) factor */
        if (((n - k) & 1) != 0) {
            term = -term;
        }

        sum += term;
    }

    seq_t denom = (seq_t)(n + 1);   /* cast from idx_t to seq_t */
    return sum / denom;
}
