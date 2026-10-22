/*
 * A361824  Sum of odd middle divisors of n, where "middle divisor" means a
 *          divisor in the half-open interval [sqrt(n/2), sqrt(n*2)).
 * https://oeis.org/A361824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of odd divisors d of n with sqrt(n/2) <= d < sqrt(2n)
 * Range:     u8 n <= 3824, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* idx_t and seq_t are defined by the build environment */

static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__uint128_t)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for index below offset */

    /* Lower bound L: smallest d such that 2*d*d >= n   (ceil(sqrt(n/2))) */
    idx_t low = 1, high = isqrt(n) + 1, mid, L = 1;
    while (low <= high) {
        mid = (low + high) >> 1;
        if ((__uint128_t)mid * mid * 2 >= n) {
            L = mid;
            high = mid - 1;
        } else
            low = mid + 1;
    }

    /* Upper bound U: largest d such that d*d < 2*n   (floor(sqrt(2n)) exclusive) */
    low = 1; high = n;
    idx_t U = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if ((__uint128_t)mid * mid < (__uint128_t)2 * n) {
            U = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }

    seq_t sum = 0;
    idx_t sq = isqrt(n);          /* iterate divisors up to sqrt(n) */
    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;

        if (d >= L && d <= U && (d & 1)) sum += (seq_t)d;
        if (e != d && e >= L && e <= U && (e & 1)) sum += (seq_t)e;
    }
    return sum;
}
