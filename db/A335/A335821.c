/*
 * A335821  Triangular array T(n, k) = n^2 - k^2, read by rows.
 * https://oeis.org/A335821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r^2 - k^2, where r = floor((sqrt(8*n+1)-1)/2) k = n -
 *            r*(r+1)/2 (i.e., the element n in the flattened triangle of n^2
 *            - k^2)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the largest r such that r*(r+1)/2 <= n */
static inline idx_t _find_row(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;

    /* Increase hi until the triangular number exceeds n */
    while (( (__int128)hi * (hi + 1) / 2) <= n) {
        hi <<= 1;
    }

    /* Binary search between lo and hi */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (( (__int128)mid * (mid + 1) / 2) <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    const idx_t r = _find_row(n);

    /* Start index of row r in the flattened sequence */
    const idx_t start = (idx_t)(((__int128)r * (r + 1)) / 2);
    const idx_t k = n - start;

    /* Perform arithmetic in seq_t after determining r and k */
    const seq_t rval = (seq_t) r;
    const seq_t kval = (seq_t) k;

    return rval * rval - kval * kval;
}
