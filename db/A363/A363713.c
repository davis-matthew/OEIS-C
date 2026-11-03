/*
 * A363713  Number of ways to write n as sum of a positive square and a
 *          positive fifth power.
 * https://oeis.org/A363713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 106 terms through n = 106 (2026-11-01)
 * Helpers:   ipow_i32, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = #{ (k,m) | k >= 1, m >= 1, k^2 + m^5 = n } 
 * i.e. the number of ways to write n as the sum of a positive square
 * and a positive fifth power.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_u64_idx.h"

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
    /* At least 1^2 + 1^5 = 2, so for n < 2 the answer is 0. */
    if (n < 2)
        return 0;

    seq_t count = 0;
    seq_t N      = (seq_t)n;

    for (idx_t m = 1; ; ++m) {
        seq_t power = ipow_u64_idx((uint64_t)m, 5);   /* m^5 */
        if (power >= N)
            break;

        seq_t rem = N - power;                  /* n - m^5 */
        /* rem >= 1 automatically because power < N */

        seq_t s = floor_sqrt(rem);              /* floor(sqrt(rem)) */
        if (s * s == rem && s >= 1)
            ++count;
    }

    return count;
}
