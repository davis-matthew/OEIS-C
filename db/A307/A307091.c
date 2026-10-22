/*
 * A307091  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(n,2*k)^2.
 * https://oeis.org/A307091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (-1)^k * binomial(n, 2k)^2
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 19, i64 n <= 38, i128 n <= 74,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t r   = k << 1;                     /* r = 2k */
        seq_t c   = binomial(n, r);             /* binomial(n,2k) */
        seq_t term = c * c;                     /* square the binomial */
        if (k & 1ULL)   /* odd k => subtract */
            sum -= term;
        else            /* even k => add */
            sum += term;
    }
    return sum;
}
