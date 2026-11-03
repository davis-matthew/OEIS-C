/*
 * A366519  Largest odd divisor of n which is <= sqrt(n).
 * https://oeis.org/A366519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest odd divisor of n that is <= sqrt(n)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9987 (2026-11-01)
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
    /* n >= 1 as per OEIS offset */
    seq_t nn = (seq_t)n;                    /* work in seq_t to avoid overflow */
    seq_t sq = floor_sqrt(nn);              /* floor(sqrt(n)) */

    /* Search downwards for the largest odd divisor <= sqrt(n) */
    for (seq_t d = sq; d > 0; --d)
    {
        if ((d & 1) == 1 && (nn % d) == 0)
            return d;
    }

    /* The loop always finds at least 1, but the return here
       satisfies the compiler for completeness. */
    return 1;
}
