/*
 * A332932  Sum of ceiling(sqrt(d)) where d runs through the divisors of n.
 * https://oeis.org/A332932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} ceil(sqrt(d))
 * Range:     u8 n <= 1079, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor square root for uint64_t */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        /* avoid overflow: mid*mid > n iff mid > n/mid */
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* integer ceil square root for uint64_t */
static inline idx_t ceil_sqrt_u64(idx_t n)
{
    idx_t f = floor_sqrt_u64(n);
    return (f * f == n) ? f : (f + 1);
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) /* not needed, but harmless */
        return sum;
    idx_t sqrt_n = floor_sqrt_u64(n);
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            sum += (seq_t)ceil_sqrt_u64(d);
            if (q != d)
                sum += (seq_t)ceil_sqrt_u64(q);
        }
    }
    return sum;
}
