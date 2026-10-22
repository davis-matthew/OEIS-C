/*
 * A374061  Number of solutions to a^2 + n = 2 * b^2 with 0 < a < b.
 * https://oeis.org/A374061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{(a,b): 0 < a < b and a^2 + n = 2·b^2}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static idx_t isqrt_idx(idx_t n)
{
    /* integer square root using binary search without overflow */
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially increase high until high^2 > n */
    while (high <= n / high) {
        high <<= 1;
    }
    low = high >> 1;          /* high is now > sqrt(n) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid <= n / mid) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;                /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;             /* offset is 1 */

    idx_t count = 0;
    idx_t n_i   = n;

    /* Determine b-range: sqrt(n/2) < b < sqrt(n)  */
    idx_t b_min = isqrt_idx(n_i / 2) + 1;
    idx_t b_max = isqrt_idx(n_i - 1);          /* floor(sqrt(n-1)) */

    for (idx_t b = b_min; b <= b_max; ++b) {
        idx_t rhs = 2 * b * b - n_i;           /* must fit in idx_t (<= 2^64-1) */
        if (rhs <= 0)
            continue;                         /* a^2 must be positive */
        idx_t a_val = isqrt_idx(rhs);
        if (a_val * a_val == rhs && a_val < b)
            ++count;
    }

    return (seq_t)count;
}
