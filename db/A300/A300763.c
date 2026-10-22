/*
 * A300763  a(n) = ceiling(n/g^3), where g = (1+sqrt(5))/2 is the golden
 *          ratio.
 * https://oeis.org/A300763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( n / (2 + sqrt(5)) ) = floor( n / (2 + sqrt(5)) ) +
 *            1 For n = 0 return 0. The following implementation uses integer
 *            arithmetic only, comparing k*(2 + sqrt(5)) <= n by avoiding sqrt
 *            via k^2*5 <= (n - 2k)^2. The value is found with a binary search
 *            in O(log n) time
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* binary search for k = floor(n / (2 + sqrt(5))) */
    idx_t lo = 0, hi = n;          /* hi may be reduced; f(n) <= n-1 for n>0 */

    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* candidate */

        /* check if mid*(2 + sqrt(5)) <= n  */
        /* equivalently k^2 * 5 <= (n - 2k)^2 with n-2k >= 0 */

        int64_t diff = (int64_t)n - (int64_t)(2 * mid);
        if (diff >= 0) {
            __uint128_t lhs = (__uint128_t)mid * mid * 5;
            __uint128_t rhs = (__uint128_t)diff * diff;
            if (lhs <= rhs)
                lo = mid;            /* mid is feasible, try larger */
            else
                hi = mid - 1;        /* mid too large */
        } else {
            hi = mid - 1;            /* mid too large because n-2k < 0 */
        }
    }

    /* a(n) = k + 1   (ceil)  */
    return (seq_t)(lo + 1);
}
