/*
 * A375241  Nontriangular numbers with digital root in {1, 3, 6, 9}.
 * https://oeis.org/A375241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer that is not a triangular number *
 *            and whose digital root is 1, 3, 6, or 9
 * Range:     u8 none, u16 n <= 305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9198 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

/* Helper: true if x has digital root 1,3,6, or 9 (i.e. x % 9 == 0,1,3,6). */
static inline bool desired_dr(seq_t x)
{
    seq_t r = x % 9;
    return r == 0 || r == 1 || r == 3 || r == 6;
}

/* Helper: test whether x is a triangular number.
 * x is triangular iff 1 + 8*x is a perfect square and its square root is odd. */
static inline bool is_triangular(seq_t x)
{
    seq_t d = 1 + (seq_t)8 * x;        /* discriminant */
    seq_t s = floor_sqrt(d);
    return (s * s == d) && (s & 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; 0 is not used but safe */
    idx_t   count = 0;
    seq_t   num   = 0;            /* current integer being tested */

    while (count < n)
    {
        ++num;
        if (desired_dr(num) && !is_triangular(num))
            ++count;
    }
    return num;
}
