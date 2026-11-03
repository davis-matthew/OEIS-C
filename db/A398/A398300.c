/*
 * A398300  a(n) is the minimum possible sum of the numbers on all non-mine
 *          cells when n mines are placed on an infinite Minesweeper board
 *          (each non-mine cell's number is the count of mines among its up to
 *          8 neighbors).
 * https://oeis.org/A398300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*ceiling(sqrt(28*n - 12))
 * Range:     u8 n <= 2, u16 n <= 483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9594 (2026-11-01)
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
    /* 28*n - 12 in seq_t */
    seq_t t = (seq_t)28 * (seq_t)n - (seq_t)12;

    /* floor sqrt */
    seq_t s = floor_sqrt(t);

    /* if not perfect square, take ceil */
    if (s * s < t) s += 1;

    return (seq_t)2 * s;
}
