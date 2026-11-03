/*
 * A359696  a(n) is the number of points with integer coordinates located
 *          between the x-axis and the graph of the function y = n^3 / (n^2 +
 *          x^2).
 * https://oeis.org/A359696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 2*Sum_{y=1}^{n-1} floor(sqrt(n^3 / y - n^2))
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 426, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9553 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only. */
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

seq_t a(idx_t n) {
    /* n is unsigned. */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t sum = 0;
    for (idx_t y = 1; y < n; ++y) {
        seq_t yt = (seq_t)y;
        seq_t num = n3 - n2 * yt;      /* n^3 - n^2*y */
        seq_t t   = num / yt;          /* floor(n^3 / y - n^2) as integer */
        seq_t s   = floor_sqrt(t);     /* floor(sqrt(t)) */
        sum += s;
    }
    return nn + 2 * sum;
}
