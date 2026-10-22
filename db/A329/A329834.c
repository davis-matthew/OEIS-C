/*
 * A329834  Beatty sequence for (11+sqrt(73))/8.
 * https://oeis.org/A329834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (11 + sqrt(73))/8)
 * Range:     u8 none, u16 n <= 14, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Beatty sequence for (11+sqrt(73))/8.
 * a(1) = 2, a(2) = 4, a(3) = 7, ... */
seq_t a(idx_t n)
{
    /* convert index to the integer type used for the sequence */
    seq_t n_t = (seq_t)n;

    /* compute floor(n * sqrt(73)) as floor_sqrt(73 * n^2) */
    seq_t prod = (seq_t)73 * n_t * n_t;
    seq_t q = floor_sqrt(prod);

    /* numerator of the fraction: 11*n + floor(n*sqrt(73)) */
    seq_t numer = (seq_t)11 * n_t + q;

    /* final result: floor( (11*n + floor(n*sqrt(73))) / 8 ) */
    return numer / (seq_t)8;
}
