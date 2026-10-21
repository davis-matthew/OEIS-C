/*
 * A279417  Triangle read by rows: T(n,k), n >= k >= 1, is the number of grid
 *          points on or below the diagonal of an n X k grid.
 * https://oeis.org/A279417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 6490, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A279417 (triangle read by rows).
   Each term is T(r,k)= (k*r + (gcd(r-1,k-1)+1))/2.  The row r
   and column k are obtained from the global index n. */
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Find the row r where n lies: r*(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cnt = (seq_t)mid * (mid + 1) / 2;
        if (cnt >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms preceding row r */
    idx_t prev = r * (r - 1) / 2;

    /* Column index within the row (1‑based) */
    idx_t k = n - prev;

    /* Compute the triangle element */
    seq_t g = gcd((seq_t)(r - 1), (seq_t)(k - 1));
    return ((seq_t)k * (seq_t)r + (g + 1)) / 2;
}
