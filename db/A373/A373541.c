/*
 * A373541  Expansion of e.g.f. exp(x/(1 + x^4)).
 * https://oeis.org/A373541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/4⌋} (-1)^k * binomial(n-3*k-1,
 *            k)/(n-4*k)!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the explicit O(n/4) summation. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"  // provides seq_t binomial(idx_t n, idx_t k)

seq_t a(idx_t n)
{
    /* maximum k such that n-4*k >= 0  */
    idx_t maxk = n / 4;
    seq_t sum = 0;
    seq_t prod = 1;                 /* prod = n!/(n-4*k)!  */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* term = (-1)^k * binomial(n-3*k-1, k) * prod */
        idx_t N = n - 3 * k - 1;
        seq_t bin = binomial(N, k);
        seq_t term = bin * prod;
        if (k & 1)            /* apply the alternating sign   */
            term = -term;
        sum += term;

        /* prepare prod for the next k (multiply by next 4 integers) */
        if (k < maxk)
        {
            idx_t start = n - 4 * (k + 1) + 1;   /* n-4k-3   */
            /* multiply by start, start+1, start+2, start+3 */
            prod *= (seq_t)(start);
            prod *= (seq_t)(start + 1);
            prod *= (seq_t)(start + 2);
            prod *= (seq_t)(start + 3);
        }
    }

    return sum;
}
