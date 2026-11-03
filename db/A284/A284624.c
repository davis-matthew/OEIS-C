/*
 * A284624  Positions of 1 in A284749.
 * https://oeis.org/A284624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*phi) + 2*(n-1) + 2
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9772 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use integer arithmetic only, relying on floor_sqrt for sqrt(5). */
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
    /* n is 1-based */
    seq_t k = (seq_t)(n - 1);          /* k = n-1 */
    seq_t s = floor_sqrt((seq_t)5 * k * k); /* s = floor(k*sqrt5) */
    seq_t b = (k + s) / 2;            /* b = floor(k*phi) */
    return b + 2 * k + 2;             /* final a(n) */
}
