/*
 * A259477  Triangle of numbers where T(n,k) is the number of k-dimensional
 *          faces on a partially truncated n-dimensional simplex, 0 <= k <= n.
 * https://oeis.org/A259477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flattened triangle of T(r,k) where r = largest r with
 *            r(r+1)/2 <= n, k = n - r(r+1)/2
 * Range:     u8 n <= 29, u16 n <= 124, u32 n <= 477, u64 n <= 1979,
 *            u128 n <= 7933, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r using binary search.  Use 128‑bit arithmetic to
       avoid overflow for very large n. */
    idx_t low  = 0;
    idx_t high = 1;
    while (((unsigned __int128)high * (high + 1ULL)) / 2 <= n)
        high <<= 1;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (((unsigned __int128)mid * (mid + 1ULL)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Index within the row. */
    idx_t first_of_row = (r * (r + 1)) / 2;
    idx_t k = n - first_of_row;

    if (r == 0) {
        return (seq_t)1;                      /* T(0,0) = 1 */
    }
    if (k == 0) {
        return (seq_t)r * ((seq_t)r + 1);     /* T(r,0) = r(r+1) */
    }
    return ((seq_t)(r + 1 - k)) * binomial(r + 1, k + 1);  /* T(r,k) */
}
