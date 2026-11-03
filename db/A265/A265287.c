/*
 * A265287  Number of steps needed to reach 1 in the "sqrt(2)*x + 1" problem.
 * https://oeis.org/A265287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 1 in the sqrt(2)*x + 1 problem
 *            (starting with x = n)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 516, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9734 (2026-11-01)
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
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x > 1)
    {
        if ((x & 1) == 0)
        {
            x >>= 1;                 /* even case: divide by 2 */
        }
        else
        {
            /* odd case: floor(x*sqrt(2) + 1)
               Integer part of x*sqrt(2) is floor(sqrt(2*x*x)) */
            seq_t t = x * x;
            t = t * 2;
            x = floor_sqrt(t) + 1;
        }
        steps++;
    }
    return steps;
}
