/*
 * A366523  Largest square divisor of n which is <= sqrt(n).
 * https://oeis.org/A366523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9358 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the largest square divisor d of n such that d ≤ sqrt(n)
 */
#include <stdbool.h>
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
    /* n is index ≥1; convert to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* r = floor(sqrt(n)) */
    seq_t r = floor_sqrt(nn);

    /* s_bound = floor(sqrt(r)) → largest possible square root of a square divisor ≤ r */
    seq_t bound = floor_sqrt(r);

    /* iterate from bound downward to find the first square that divides n */
    for (idx_t s = (idx_t)bound; s >= 1; --s)
    {
        seq_t sq = (seq_t)s * (seq_t)s;   /* s² */
        if (nn % sq == 0)
            return sq;                    /* largest satisfying square divisor found */
    }

    /* should never reach here: at least 1 divides everything */
    return (seq_t)1;
}
