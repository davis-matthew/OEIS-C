/*
 * A286666  Positions of 0 in A286665; complement of A286667.
 * https://oeis.org/A286666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(2)) + n - 1 a(1) = 1, a(2) = 3, a(3) = 6, …
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > x / mid)   /* mid*mid > x */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n is 1-indexed, guaranteed n >= 1 */
    uint64_t n64 = n;                    /* use 64‑bit arithmetic for safety   */
    uint64_t m   = 2ULL * n64 * n64;     /* m = 2 * n^2                          */
    uint64_t s   = floor_sqrt_u64(m);    /* floor(sqrt(2*n^2)) = floor(n*sqrt(2)) */
    uint64_t res = s + n64 - 1;
    return (seq_t)res;
}
