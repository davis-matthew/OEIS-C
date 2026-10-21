/*
 * A292638  Rank of (3-r)*n when all the numbers (3-r)*j and (3+r)*k, where r
 *          = sqrt(5), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((7*n - floor(3*n*sqrt(5)) - 1)/2)
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* compute floor(3*n*sqrt(5)) as floor_sqrt(5*(3*n)^2) */
    seq_t k   = N * (seq_t)3;          /* 3*n */
    seq_t k2  = k * k;                 /* (3*n)^2 */
    seq_t prod = k2 * (seq_t)5;        /* 5*(3*n)^2 */
    seq_t f = floor_sqrt(prod);        /* floor(3*n*sqrt(5)) */

    /* remaining arithmetic */
    seq_t t = (seq_t)7 * N - f - (seq_t)1;  /* 7*n - f - 1 */
    seq_t t_div2 = t / (seq_t)2;           /* floor((7*n - f - 1)/2) */

    return N + t_div2;
}
