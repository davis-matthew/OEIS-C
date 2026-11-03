/*
 * A372644  For n >= 1, a(n) is the least k >= 1 such that for some x >= 1,
 *          n^2 - k^2 = x^3 or a(n) = -1 if no such k exists.
 * https://oeis.org/A372644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 64, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n): the least k≥1 such that n^2−k^2 = x^3 for some
    positive integer x; returns -1 if no such k exists.
    Algorithm: iterate over cubes ≤ n^2−1 starting from the largest,
    stop at first cube for which n^2−cube is a perfect square (k).  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
    if (n < 1) return -1;                               // safety
    seq_t n2 = (seq_t)n * (seq_t)n;                    // n²

    seq_t max_cube_root = floor_cube_root(n2 - 1);      // largest x with x³ < n²
    if (max_cube_root == 0) return -1;                 // n=1

    idx_t xmax = (idx_t)max_cube_root;
    for (idx_t x = xmax; x >= 1; --x) {
        seq_t cube = (seq_t)x * (seq_t)x * (seq_t)x;    // x³
        seq_t diff = n2 - cube;                       // n² - x³ > 0
        seq_t r    = floor_sqrt(diff);                // ⌊√diff⌋
        if (r * r == diff) {                          // diff is a square
            return r;                                 // k = √diff
        }
    }
    return -1;                                         // no solution
}
