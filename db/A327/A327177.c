/*
 * A327177  a(n) = [(2n+1)r] - [(n+1)r] - [nr], where [ ] = floor and r =
 *          sqrt(2).
 * https://oeis.org/A327177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+1)*√2) - floor((n+1)*√2) - floor(n*√2)
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static inline seq_t floor_sqrt2_mult(idx_t k)
{
    /* Compute floor(k * √2) as floor(√(2 * k * k)) */
    return floor_sqrt((seq_t)2 * (seq_t)k * (seq_t)k);
}

seq_t a(idx_t n)
{
    seq_t s_n       = floor_sqrt2_mult(n);          /* floor(n √2)          */
    seq_t s_n_plus1 = floor_sqrt2_mult(n + 1);      /* floor((n+1) √2)      */
    seq_t s_2n_plus1 = floor_sqrt2_mult(2 * n + 1); /* floor((2n+1) √2)     */

    return s_2n_plus1 - s_n_plus1 - s_n;
}
