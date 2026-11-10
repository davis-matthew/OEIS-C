/*
 * A345999  a(n) = (m+1)/gcd(m+1,n), where m = A344005(n).
 * https://oeis.org/A345999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m+1)/gcd(m+1,n), where m = A344005(n) = least m >= 1
 *            with n | m*(m+1); found by linear search
 * Range:     u8 n <= 2318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9387 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A345999_gcd(uint64_t x, uint64_t y)
{
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t m;
    if (N == 0) return (seq_t)0;
    for (m = 1; m < N; m++) {
        unsigned __int128 p = (unsigned __int128)m * (unsigned __int128)(m + 1);
        if ((uint64_t)(p % N) == 0) break;
    }
    /* m == N is always a solution if the loop finishes without a break */
    {
        uint64_t m1 = m + 1;
        uint64_t g = A345999_gcd(m1, N);
        return (seq_t)(m1 / g);
    }
}
