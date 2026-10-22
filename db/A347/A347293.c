/*
 * A347293  Triangle read by rows: T(n, k) = Sum_{i=1..n} gcd(1 + (i-1) *
 *          (k-1),n) for 1 <= k <= n.
 * https://oeis.org/A347293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where T(r,k)=∑_{i=1}^{r}gcd(1+(i-1)*(k-1),r), r =
 *            smallest integer with r(r+1)/2 ≥ n, k = n - r(r-1)/2, 1 ≤ k ≤ r
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* find r via binary search to avoid overflow */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* compute k = n - C(r-1) */
    unsigned __int128 prev = (__int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)prev;

    /* compute the sum of gcds */
    seq_t sum = 0;
    for (idx_t i = 1; i <= r; ++i) {
        seq_t term = (seq_t)1 + (seq_t)(i - 1) * (seq_t)(k - 1);
        seq_t g = gcd(term, (seq_t)r);
        sum += g;
    }
    return sum;
}
