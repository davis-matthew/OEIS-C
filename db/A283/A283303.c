/*
 * A283303  List points (x,y) having integer coordinates with x >= y >= 0,
 *          sorted first by x^2+y^2 and in case of a tie, by x-coordinate.
 *          Sequence gives x-coordinates.
 * https://oeis.org/A283303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the x‑coordinate of the nth point in A283303
   by brute‑force search: iterate over increasing sum s = x^2 + y^2,
   enumerating all pairs (x,y) with 0 ≤ y ≤ x and y^2 = s – x^2.
   The sequence is indexed from 1. */
#include <stdint.h>

static inline idx_t isqrt(idx_t v)
{
    idx_t lo = 0;
    idx_t hi = v < 0xFFFFFFFFULL ? v : 0xFFFFFFFFULL; /* sqrt(max uint64) < 2^32 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* n is 1‑based, so we want the nth point. */
    idx_t target = n;
    idx_t count  = 0;       /* number of points seen so far */

    for (idx_t s = 0;; ++s) {
        idx_t limit = isqrt(s);          /* x and y cannot exceed sqrt(s)   */
        for (idx_t x = 0; x <= limit; ++x) {
            idx_t rem = s - x * x;      /* remainder that must be a square */
            idx_t y   = isqrt(rem);
            if (y * y == rem && y <= x) {
                ++count;
                if (count == target) {
                    return (seq_t)x;
                }
            }
        }
    }
}
