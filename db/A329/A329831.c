/*
 * A329831  Beatty sequence for (7+sqrt(65))/8.
 * https://oeis.org/A329831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(65))/8)
 * Range:     u8 none, u16 n <= 15, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset is 1; return 0 for safety
    seq_t nn = (seq_t)n;           // cast index to seq_t
    seq_t sq = nn * nn;            // compute n^2
    sq *= (seq_t)65;               // multiply by 65
    seq_t sqrt_part = floor_sqrt(sq); // floor(n*sqrt(65))
    seq_t result = (sqrt_part + nn * (seq_t)7) / (seq_t)8; // (floor(n*sqrt(65))+7n)/8
    return result;
}
