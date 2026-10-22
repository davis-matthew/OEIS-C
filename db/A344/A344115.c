/*
 * A344115  Triangle read by rows: T(n,k) is the number of relations from an
 *          n-element set to a k-element set that are not one-to-one
 *          functions.
 * https://oeis.org/A344115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 20, u64 n <= 43, u128 n <= 76,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is the (n)th entry of the triangle read by columns.
   Let k be the smallest integer with k*(k+1)/2 >= n.
   Let r = n - (k-1)*k/2   (1 <= r <= k).
   Then T(r,k) = number of non one‑to‑one relations from r‑set to k‑set,
   i.e. 2^(r*k) - k!/(k-r)!   (falling factorial).
   The function returns T(r,k).
*/

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find column k, satisfying k*(k+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t k = lo;

    /* Previous cumulative terms */
    idx_t prev = (idx_t)((__int128)(k - 1) * k / 2);
    /* Position within the column (0‑based) */
    idx_t pos = n - prev - 1;
    /* Row index r = pos + 1 */
    idx_t r = pos + 1;

    /* Compute 2^(r*k) */
    idx_t exp = (idx_t)((__int128)r * k);
    seq_t power = pow2(exp);

    /* Compute falling factorial k!/(k-r)! = product_{i=0}^{r-1} (k-i) */
    seq_t ff = 1;
    for (idx_t i = 0; i < r; ++i)
        ff *= (seq_t)(k - i);

    return power - ff;
}
