/*
 * A328819  Third digit after decimal point of square root of n.
 * https://oeis.org/A328819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor_sqrt(1000000*n) % 10
 * Range:     u8 none, u16 none, u32 n <= 281, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 116 terms through n = 9902 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Third digit after decimal point of sqrt(n)   */

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
    seq_t prod = (seq_t)n * (seq_t)1000000u;
    seq_t root = floor_sqrt(prod);
    return root % 10;
}
