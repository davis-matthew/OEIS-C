/*
 * A286992  Positions of 1 in A286990; complement of A286991.
 * https://oeis.org/A286992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least N such that A286990 has n ones among its first N
 *            terms (positions of 1 in A286990); found by binary search with a
 *            closed-form prefix count, using g(m) = floor(m/sqrt(2)) =
 *            isqrt(floor(m^2/2))
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9966 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A286992_isqrt(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 63;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

/* floor(m/sqrt(2)) */
static uint64_t A286992_g(uint64_t m)
{
    unsigned __int128 s = (unsigned __int128)m * m / 2;
    return A286992_isqrt(s);
}

/* number of ones among first N terms of A286990 */
static uint64_t A286992_count(uint64_t N)
{
    uint64_t k = N / 3, r = N % 3;
    uint64_t ones = k + A286992_g(k + 1);
    if (r > 0) {
        uint64_t orig = k + 1;
        uint64_t bit = A286992_g(orig + 1) - A286992_g(orig);
        if (bit) ones += 1;               /* 101: r=1 ->1, r=2 ->1 */
        else if (r == 2) ones += 1;       /* 010: r=1 ->0, r=2 ->1 */
    }
    return ones;
}

seq_t a(idx_t n)
{
    uint64_t lo = 1, hi = 2 * (uint64_t)n + 2;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A286992_count(mid) >= (uint64_t)n) hi = mid; else lo = mid + 1;
    }
    return (seq_t)lo;
}
