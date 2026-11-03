/*
 * A329831  Beatty sequence for (7+sqrt(65))/8.
 * https://oeis.org/A329831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(65))/8)
 * Range:     u8 none, u16 n <= 15, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
