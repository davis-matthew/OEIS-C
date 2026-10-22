/*
 * A366046  Expansion of (1/x) * Series_Reversion( x*(1-x+x^5) ).
 * https://oeis.org/A366046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/5⌋} (-1)^k * binom(n+k,k) * *
 *            binom(2*n-4*k, n-5*k) (OEIS A366046)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t,n,k) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t sum = (seq_t)0;
    idx_t max_k = n / 5;                  /* floor(n/5) */

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t b1 = binomial(n + k, k);
        seq_t b2 = binomial((2 * n) - (4 * k), n - (5 * k));
        seq_t term = b1 * b2;
        if (k & 1)
            sum -= term;                  /* (-1)^k = -1 for odd k */
        else
            sum += term;                  /* (+1) for even k */
    }

    seq_t denom = (seq_t)(n + 1);         /* 1/(n+1) division */
    return sum / denom;
}
