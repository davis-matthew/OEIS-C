/*
 * A391490  Denominator of k + a/k where n = k^2 + 2a + 1 and -k <= a <= k.
 * https://oeis.org/A391490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A391490:  a(n) is the denominator of k + a/k where
 * n = k^2 + 2a + 1 with -k ≤ a ≤ k.
 * That denominator equals k / gcd(k, a).
 */

#include <stdint.h>

/* integer square root of a non‑negative idx_t, floor(sqrt(x)) */
static idx_t isqrt_idx(idx_t x)
{
    idx_t low = 0;
    idx_t high = (x >> 1) + 1;          /* high is an upper bound for sqrt(x) */
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Euclidean gcd for idx_t values */
static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Search for a k that satisfies n = k^2 + 2a + 1 with -k ≤ a ≤ k. */
    idx_t kmax = isqrt_idx(n - 1) + 2;   /* safe upper bound for k */
    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* a = (n - k^2 - 1) / 2, computed with signed 128‑bit to allow negativity */
        __int128 diff = (__int128)n - (__int128)k * (__int128)k - 1;
        if ((diff & 1) != 0)            /* diff must be even for integer a */
            continue;

        __int128 a_val = diff >> 1;     /* same as diff / 2 */

        /* Check |a| ≤ k */
        if (a_val < -(__int128)k || a_val > (__int128)k)
            continue;

        idx_t a_abs = (a_val < 0) ? (idx_t)(-a_val) : (idx_t)a_val;
        idx_t g = gcd_u64(k, a_abs);
        idx_t denom = k / g;
        return (seq_t)denom;
    }

    /* According to the definition a solution should always exist for n ≥ 1. */
    return (seq_t)1;
}
