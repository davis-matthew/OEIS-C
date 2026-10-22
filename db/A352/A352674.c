/*
 * A352674  Beatty sequence for (3/2)(1+sqrt(3)).
 * https://oeis.org/A352674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3/2)*(1+sqrt(3))*n)
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 3 * n as an integer that is large enough to hold the intermediate
     * calculations without overflow (uint64_t works for all test cases). */
    uint64_t m64 = (uint64_t)n * 3ULL;

    /* t = m^2 * 3, then convert to seq_t for the integer square root. */
    uint64_t t64 = m64 * m64 * 3ULL;
    seq_t p = floor_sqrt((seq_t)t64);          /* p = floor(m * sqrt(3)) */

    /* a(n) = floor((m + p) / 2) */
    seq_t sum = (seq_t)m64 + p;
    return sum / (seq_t)2;
}
