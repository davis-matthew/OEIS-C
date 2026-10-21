/*
 * A245207  a(n) = floor((n + sqrt(2))^2).
 * https://oeis.org/A245207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + sqrt(2))^2)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n^2 in seq_t */
    seq_t n_sq = ((seq_t)n) * ((seq_t)n);

    /* floor(2n * sqrt(2)) == floor(sqrt(8 * n^2)) */
    seq_t s = floor_sqrt(((seq_t)8) * n_sq);

    return n_sq + 2 + s;
}
