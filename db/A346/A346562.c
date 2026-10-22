/*
 * A346562  Irregular triangle read by rows in which row n lists the first n -
 *          2 terms of A000005 together with the sum of A000005(n-1) and
 *          A000005(n), with a(1) = 1.
 * https://oeis.org/A346562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1) = 1. For n >= 2, find the minimal integer r >= 2 such that
 *            tri(r-1)+1 >= n (tri(k) = k*(k+1)/2). Let prev = tri(r-2)+1 (the
 *            cumulative number of terms before row r). Let offset = n - prev.
 *            If offset <= r-2, a(n) = d(offset), the divisor function of
 *            offset. Otherwise a(n) = d(r-1) + d(r)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

static seq_t divisor_count(idx_t n)
{
    if (n <= 1) return 1;
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (m > 1) result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    /* Determine row r: minimal r >= 2 with (r-1)*r/2 + 1 >= n */
    idx_t t = n - 1;              /* t = n-1 */
    idx_t sq = isqrt_u64(8 * t + 1);
    idx_t k = (sq - 1) >> 1;      /* floor of (sqrt(8t+1)-1)/2 */
    while ((k * (k + 1)) >> 1 < t) ++k;
    idx_t r = k + 1;              /* r >= 2 */

    /* cumulative terms before row r */
    idx_t prev = ((r >= 3) ? ((r - 2) * (r - 1) / 2 + 1) : 1);
    idx_t offset = n - prev;      /* 1-based within the row */

    if (offset <= r - 2) {
        return divisor_count(offset);
    } else {
        return divisor_count(r - 1) + divisor_count(r);
    }
}
