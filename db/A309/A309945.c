/*
 * A309945  a(n) = floor(n - sqrt(2*n-1)).
 * https://oeis.org/A309945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 - floor(sqrt(2*n-2))
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 2*n - 2 as a seq_t value to avoid overflow. */
    seq_t m = ((seq_t)n) * (seq_t)2 - (seq_t)2;
    /* floor(sqrt(m)) */
    seq_t s = floor_sqrt(m);
    /* a(n) = n - 1 - s */
    seq_t res = ((seq_t)n) - (seq_t)1 - s;
    return res;
}
