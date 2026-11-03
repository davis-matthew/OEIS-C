/*
 * A357754  a(n) is the largest square with n binary digits.
 * https://oeis.org/A357754
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(sqrt(2^n - 1))^2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 35 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

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
    seq_t val = ipow_idx((seq_t)2, n);
    val -= (seq_t)1;
    seq_t s = floor_sqrt(val);
    return s * s;
}
