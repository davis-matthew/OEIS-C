/*
 * A309680  The smallest nonsquare nonzero integer that is a quadratic residue
 *          modulo n, or 0 if no such integer exists.
 * https://oeis.org/A309680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest non‑zero integer that is a quadratic residue
   modulo n but is not a perfect square.  Returns 0 if no such integer
   exists. */
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
    if (n <= 1)
        return (seq_t)0;

    seq_t min_val = (seq_t)0;
    bool found = false;

    for (idx_t y = 1; y < n; ++y) {
        seq_t r = (seq_t)y * (seq_t)y % (seq_t)n;
        if (r == (seq_t)0)          /* skip residue zero */
            continue;

        seq_t s = floor_sqrt(r);
        if (s * s == r)             /* skip perfect square integers */
            continue;

        if (!found || r < min_val) {
            min_val = r;
            found = true;
            if (min_val == (seq_t)2)   /* minimal possible value */
                break;
        }
    }

    return found ? min_val : (seq_t)0;
}
