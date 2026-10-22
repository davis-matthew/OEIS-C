/*
 * A370841  Expansion of (1/x) * Series_Reversion( x * (1/(1-x^4) - x) ).
 * https://oeis.org/A370841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} (-1)^k *
 *            binomial(2*n-4*k+1,k) * binomial(2*n-4*k,n-4*k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binomial((2*n) - 4*k + 1, k);
        seq_t term2 = binomial((2*n) - 4*k,      n - 4*k);
        seq_t prod  = term1 * term2;
        if (k & 1)  sum -= prod;
        else        sum += prod;
    }
    seq_t denom = (seq_t)n + 1;
    return sum / denom;
}
