/*
 * A370837  Expansion of (1/x) * Series_Reversion( x/(x+1/(1+x^3)) ).
 * https://oeis.org/A370837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = \sum_{k=0}^{\lfloor n/3 \rfloor} (-1)^k binomial(n,3k) *
 *            binomial(4k,k) / (3k+1)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 21, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k * 3 <= n; ++k) {
        seq_t term = binomial(n, 3 * k);
        term *= binomial(4 * k, k);
        term /= (seq_t)(3 * k + 1);
        if (k & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
