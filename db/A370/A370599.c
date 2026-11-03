/*
 * A370599  a(n) is the number of distinct triangles with integral
 *          side-lengths for which the perimeter 2*n divides the area.
 * https://oeis.org/A370599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 377, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 103 terms through n = 489 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the distinct integer triangles with perimeter 2*n
   for which the area A is an integer multiple of the perimeter.
   The condition reduces to:
        A = sqrt( n*(n-a)*(n-b)*(n-c) )
        A % (2*n) == 0   and   sqrt(...) must be integral.
   We enumerate a ≤ b ≤ c, a+b+c = 2*n, and check the triangle
   inequality and the area condition.  Complexity is O(n²). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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
    if (n == 0) return 0;          /* undefined case, return 0 */
    seq_t count = 0;
    seq_t two_n = (seq_t)2 * n;    /* perimeter */

    /* a ranges from 1 to floor(2*n/3) */
    idx_t a_max = (2 * n) / 3;
    for (idx_t a = 1; a <= a_max; ++a) {
        /* b ranges from a to (2*n - a)/2 to keep b ≤ c */
        idx_t b_max = (2 * n - a) / 2;
        for (idx_t b = a; b <= b_max; ++b) {
            idx_t c = 2 * n - a - b;
            if (a + b <= c) continue;          /* not a triangle */

            seq_t prod = (seq_t)n * (seq_t)(n - a) *
                         (seq_t)(n - b) * (seq_t)(n - c);
            seq_t sq = floor_sqrt(prod);
            if (sq * sq != prod) continue;     /* area not integer */

            if (sq % two_n == 0) count++;      /* perimeter divides area */
        }
    }
    return count;
}
