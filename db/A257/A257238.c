/*
 * A257238  Triangle T(n, k) = n^3 - k^3, 0 <= k < = n.
 * https://oeis.org/A257238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = row^3 - col^3, where row is the unique r such that
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2, col = n - r(r+1)/2. (This reads the
 *            triangle T(n,k)=n^3−k^3 row‑by‑row.)
 * Range:     u8 n <= 27, u16 n <= 860, u32 n <= 9767, u64 n <= 9767,
 *            u128 n <= 9767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r via binary search on r(r+1)/2 ≤ n.  */
    idx_t lo = 0;
    idx_t hi = n + 1;                     /* guarantees hi*(hi+1)/2 > n   */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t r = lo;

    /* Compute  Σ_{i=0}^{r-1} (i+1) = r(r+1)/2  */
    __int128 prev_tmp = (__int128)r * (r + 1) / 2;
    idx_t prev = (idx_t)prev_tmp;

    /* Column index */
    idx_t k = n - prev;

    /* Compute r^3 - k^3 in seq_t space */
    seq_t rr = (seq_t)r;
    rr *= (seq_t)r;
    rr *= (seq_t)r;

    seq_t kk = (seq_t)k;
    kk *= (seq_t)k;
    kk *= (seq_t)k;

    return rr - kk;
}
