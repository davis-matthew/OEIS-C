/*
 * A339354  G.f.: Sum_{k>=1} k^3 * x^(k*(k + 1)) / (1 - x^k).
 * https://oeis.org/A339354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of cubes of divisors of n that are smaller than
 *            sqrt(n)
 * Range:     u8 n <= 47, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for idx_t (floor) */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
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
    if (n == 0) return (seq_t)0;  /* not required, but safe */
    idx_t sqrt_n = isqrt(n);
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            if (d * d == n) continue;          /* skip sqrt if square */
            seq_t d_seq = (seq_t)d;
            seq_t d2 = d_seq * d_seq;
            seq_t d3 = d2 * d_seq;
            sum += d3;
        }
    }
    return sum;
}
