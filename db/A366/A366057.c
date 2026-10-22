/*
 * A366057  Expansion of (1/x) * Series_Reversion( x/(1-x+x^5) ).
 * https://oeis.org/A366057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/5)} (-1)^(n-k) *
 *            binomial(n+1,k) * binomial(n-k+1, n-5*k)
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 24, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = n / 5;

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t c1 = binomial(n + 1, k);
        seq_t c2 = binomial(n - k + 1, n - 5 * k);

        seq_t prod = c1 * c2;
        if (((n - k) & 1) != 0) {   /* (-1)^(n-k) */
            prod = -prod;
        }

        sum += prod;
    }

    return sum / ((seq_t)n + 1);
}
