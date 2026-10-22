/*
 * A321297  a(n) is the sum of proper divisors of n that are >= sqrt(n) minus
 *          the sum of the divisors that are greater than 1 and less than
 *          sqrt(n).
 * https://oeis.org/A321297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d<n, d>=sqrt(n)} d - sum_{d|n, d<n, d>1,
 *            d<sqrt(n)} d
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root, floor */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum_hi = 0;
    seq_t sum_lo = 0;
    seq_t nd = (seq_t)n;
    idx_t sq = isqrt_idx(n);

    for (idx_t i = 2; i <= sq; ++i) {
        if (n % i == 0) {
            seq_t d2 = nd / (seq_t)i;
            if ((seq_t)i == d2) {           /* perfect square divisor */
                sum_hi += (seq_t)i;
            } else {
                sum_lo += (seq_t)i;
                sum_hi += d2;
            }
        }
    }
    return sum_hi - sum_lo;
}
