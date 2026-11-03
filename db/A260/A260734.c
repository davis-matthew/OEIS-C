/*
 * A260734  a(n) = number of steps needed to reach (n^2)-1 when starting from
 *          k = ((n+1)^2)-1 and repeatedly applying the map that replaces k
 *          with k - A002828(k), where A002828(k) = the least number of
 *          squares that add up to k.
 * https://oeis.org/A260734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach (n^2)-1 from ((n+1)^2)-1
 *            * under the map k -> k - A002828(k), where A002828(k) is the
 *            minimal number * of integer squares summing to k
 * Range:     u8 n <= 7, u16 n <= 95, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: standard, pass, 111 terms through n = 1015 (2026-11-01)
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

/* test if n can be expressed as a sum of two squares.  All primes p ≡ 3 (mod 4)
 * must occur with even exponent in the prime factorization of n. */
static int can_two_squares(uint64_t n)
{
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            if ((p & 3) == 3 && (exp & 1) == 1)
                return 0;   /* cannot be a sum of two squares */
        }
    }
    if (t > 1 && (t & 3) == 3)
        return 0;
    return 1;
}

/* minimal number of squares that represent n (1 ≤ result ≤ 4).  n > 0. */
static seq_t min_squares(seq_t n)
{
    seq_t sq = floor_sqrt(n);
    if (sq * sq == n)
        return 1;

    uint64_t nn = (uint64_t)n;
    if (can_two_squares(nn))
        return 2;

    /* check if n has the form 4^a(8b+7), which guarantees 4 squares */
    uint64_t t = nn;
    while ((t & 3) == 0)      /* divide by 4 */
        t >>= 2;
    if ((t & 7) == 7)         /* remainder 7 modulo 8 */
        return 4;

    return 3;                 /* otherwise 3 squares suffice */
}

seq_t a(idx_t n)
{
    /* start from ((n+1)^2)-1, end at (n^2)-1 */
    seq_t nn      = (seq_t)(n + 1);
    seq_t start   = nn * nn - 1;
    seq_t target  = (seq_t)n * n - 1;

    seq_t count = 0;
    while (start > target) {
        seq_t step = min_squares(start);
        start -= step;
        ++count;
    }
    return count;
}
