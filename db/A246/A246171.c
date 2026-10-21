/*
 * A246171  Beatty sequence for sqrt(15).
 * https://oeis.org/A246171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(15))
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(15 * n * n)) which is equal to floor(n * sqrt(15)) */
    seq_t val = 15 * (seq_t)n * (seq_t)n;
    return floor_sqrt(val);
}
