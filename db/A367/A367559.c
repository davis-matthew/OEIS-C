/*
 * A367559  Square array T(n, k) = 2^k - n, read by ascending antidiagonals.
 * https://oeis.org/A367559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 2^(n - t(t+1)/2) - (t - (n - t(t+1)/2)) where t =
 *            floor((sqrt(8*n+1)-1)/2) is the diagonal index
 * Range:     i8 n <= 34, i16 n <= 134, i32 n <= 526, i64 n <= 2078,
 *            i128 n <= 8254, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation finds the coordinates (i,j) of the element on the
   antidiagonal read with descending i, then returns T(i,j)=2^j-i. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find t such that t(t+1)/2 <= n < (t+1)(t+2)/2 */
    idx_t lo = 0, hi = 1;
    while (((__int128)hi * (hi + 1)) / 2 <= n && hi < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t t = lo;

    /* Compute n0 = t(t+1)/2 and the offset within this diagonal */
    idx_t n0 = (idx_t)(((__int128)t * (t + 1)) / 2);
    idx_t delta = n - n0;          /* This is column index j          */
    idx_t i = t - delta;           /* Row index i                      */
    idx_t j = delta;               /* Column index j (used for 2^j)   */

    seq_t val = pow2(j) - (seq_t)i; /* T(i,j) = 2^j - i                */
    return val;
}
