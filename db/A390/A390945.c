/*
 * A390945  Burning number of the n-necklace graph.
 * https://oeis.org/A390945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(3*n))
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute m = 3*n as seq_t */
    seq_t m = (seq_t)3 * (seq_t)n;

    /* Floor of sqrt(m) */
    seq_t s = floor_sqrt(m);

    /* Check if s is exact sqrt of m */
    seq_t sq = s * s;
    return (sq == m) ? s : s + 1;
}
