/*
 * A398341  a(n) is the number of die sizes < n that can produce n as the
 *          product of two identical dice.
 * https://oeis.org/A398341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - min_{d|n} max(d, n/d) (the number of m < n such that
 *            n can be obtained as the product of two numbers between 1 and m)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for unsigned 64-bit integers */
static inline idx_t floor_sqrt_idx(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t k = n;                /* minimal m that works, starts large */
    idx_t sqrt_n = floor_sqrt_idx(n);
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;
            idx_t cand = d > other ? d : other;   /* max(d, n/d) */
            if (cand < k) k = cand;
        }
    }
    return (seq_t)n - (seq_t)k; /* a(n) = n - k */
}
