/*
 * A347162  Sum of cubes of odd divisors of n that are < sqrt(n).
 * https://oeis.org/A347162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of cubes of odd divisors of n that are < sqrt(n)
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for 64‑bit unsigned values. */
static inline idx_t floor_isqrt(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= n / mid) {          /* mid*mid <= n without overflow */
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n <= 1) return 0;

    /* largest integer strictly less than sqrt(n) */
    idx_t limit = floor_isqrt(n);
    if (limit * limit == n)            /* exclude sqrt(n) itself if n is a perfect square */
        limit--;

    for (idx_t d = 1; d <= limit; d += 2) {   /* iterate over odd divisors */
        if (n % d == 0) {
            seq_t dd = (seq_t)d;
            sum += dd * dd * dd;              /* add d^3 */
        }
    }
    return sum;
}
