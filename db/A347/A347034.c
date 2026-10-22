/*
 * A347034  Triangle read by columns: T(n,k) is the number of functions from
 *          an n-element set to a k-element set that are not one-to-one,
 *          k>=n>=1.
 * https://oeis.org/A347034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m,k) where k is the smallest integer with k(k+1)/2 ≥ n,
 *            m = n – (k-1)k/2. T(m,k) = k^m – k!/(k–m)!
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the column index k */
    idx_t lo = 1, hi = n;                 /* hi may be set to n safely */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t k = lo;

    /* Position m within column k (1‑based) */
    unsigned __int128 off = (unsigned __int128)(k - 1) * k / 2;
    idx_t offset = (idx_t)off;            /* offset ≤ k(k−1)/2 < k^2 */
    idx_t m = n - offset;                 /* 1 ≤ m ≤ k */

    /* Compute k^m */
    seq_t pow_val = seq_pow((seq_t)k, m);

    /* Compute k!/(k-m)! as product of k-m+1 … k */
    seq_t ratio = 1;
    for (idx_t i = k; i > k - m; --i) {
        ratio *= (seq_t)i;
    }

    return pow_val - ratio;
}
