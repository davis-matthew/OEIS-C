/*
 * A277644  Beatty sequence for sqrt(6)/2.
 * https://oeis.org/A277644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(6)/2)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t  = 6 * tn * tn;      /* t = 6*n^2 */
    return floor_sqrt(t) / (seq_t)2;
}
