/*
 * A369308  The number of square divisors d of n such that n/d is also a
 *          square.
 * https://oeis.org/A369308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n = 1; if n is a perfect square m², a(n) = τ(m)
 *            where τ(m) is the number of divisors of m; otherwise a(n) = 0.
 *            This follows from the OEIS definition: the number of square
 *            divisors d of n with n/d also a square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned integers.
   Returns floor(sqrt(n)). */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32; /* hi > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid == 0 || n / mid < mid)   /* mid*mid > n, avoid overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Number of divisors of m (τ(m)), computed without overflow. */
static inline seq_t divisor_count(idx_t m)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            if (i == m / i)
                ++cnt;
            else
                cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* safety for non‑positive n */
    idx_t m = int_sqrt(n);
    if (m * m != n) return (seq_t)0;              /* n is not a perfect square */
    return divisor_count(m);
}
