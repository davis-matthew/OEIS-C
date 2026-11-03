/*
 * A290342  Number of ways to write n as x^2 + 2*y^2 + z*(z+1)/2, where x is a
 *          nonnegative integer, and y and z are positive integers.
 * https://oeis.org/A290342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8270 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): number of solutions to  
   n = x^2 + 2*y^2 + z*(z+1)/2   with x >= 0, y >= 1, z >= 1.  */
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
    seq_t count = 0;
    seq_t N = (seq_t)n;           /* cast n to seq_t once */

    for (idx_t y = 1; ; ++y)
    {
        seq_t t1 = (seq_t)2 * (seq_t)y * (seq_t)y;   /* 2*y^2 */
        if (t1 > N) break;
        seq_t rem1 = N - t1;

        for (idx_t z = 1; ; ++z)
        {
            seq_t t2 = (seq_t)z * (seq_t)(z + 1) / 2;   /* z*(z+1)/2 */
            if (t2 > rem1) break;
            seq_t rem = rem1 - t2;

            seq_t x = floor_sqrt(rem);
            if (x * x == rem) count++;
        }
    }
    return count;
}
