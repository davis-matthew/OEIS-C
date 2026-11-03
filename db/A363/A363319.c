/*
 * A363319  Squares (A000290) and centered squares (A001844) sorted, including
 *          duplicates.
 * https://oeis.org/A363319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th element (in non‑decreasing order, with
 *            duplicates) of the sorted merge of the squares i^2 (i≥1) and the
 *            centered squares 2i^2−2i+1 (i≥1)
 * Range:     u8 n <= 9, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8967 (2026-11-01)
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

static inline idx_t count_squares(seq_t t)
{
    /* number of squares ≤ t */
    return (idx_t)floor_sqrt(t);
}

static inline idx_t count_centered(seq_t t)
{
    /* number of centered squares ≤ t.  For c_i = 2i^2−2i+1,
       the largest i satisfying c_i ≤ t is floor((1+√(2t−1))/2). */
    seq_t s = floor_sqrt((seq_t)2 * t - (seq_t)1);
    return (idx_t)(((seq_t)1 + s) / (seq_t)2);
}

static inline idx_t count_upto(seq_t t)
{
    return count_squares(t) + count_centered(t);
}

seq_t a(idx_t n)
{
    seq_t low  = (seq_t)1;
    seq_t high = (seq_t)1;

    /* enlarge high until the interval [1,high] contains at least n terms */
    while (count_upto(high) < n)
        high = high * (seq_t)2;

    /* binary search for the minimal t with count_upto(t) ≥ n */
    while (low < high)
    {
        seq_t mid = (low + high) / (seq_t)2;
        if (count_upto(mid) >= n)
            high = mid;
        else
            low = mid + (seq_t)1;
    }
    return low;
}
