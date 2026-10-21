/*
 * A258833  Nonhomogeneous Beatty sequence: ceiling((n + 1/4)*sqrt(2)).
 * https://oeis.org/A258833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n + 1/4)*sqrt(2)) = floor((n + 1/4)*sqrt(2) + 1)
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 4857, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /*  k = 4*n + 1  */
    seq_t k = ((seq_t)n) * 4 + 1;

    /*  y = 2 * k^2  */
    seq_t y = k * k;
    y = y * 2;

    /*  f = floor(sqrt(y))  */
    seq_t f = floor_sqrt(y);

    /*  a(n) = floor(f / 4) + 1  */
    return (f / 4) + 1;
}
