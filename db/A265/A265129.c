/*
 * A265129  Triangle read by rows, formed as the sum of the two versions of
 *          the natural numbers filling an equilateral triangle.
 * https://oeis.org/A265129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (ceil((-1 + sqrt(1+8n))/2))^2 + 1 Equivalently, find the
 *            smallest r such that r(r+1)/2 >= n then a(n) = r^2 + 1
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence is only defined for n >= 1 */
    if (n == 0)
        return 0;               /* undefined; return 0 just to be safe */

    /* find the row r such that r*(r+1)/2 >= n ;
       binary search on r. */
    idx_t lo = 1, hi = 1;
    /* grow upper bound hi until it is large enough */
    while ((seq_t)hi * (hi + 1) / 2 < (seq_t)n)
        hi = hi * 2;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((seq_t)mid * (mid + 1) / 2 >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* return r^2 + 1, computed in seq_t   */
    return (seq_t)r * (seq_t)r + 1;
}
