/*
 * A341315  Triangle read by rows: T(n,k) = (n+k)/gcd(n,k), n>=0, 0<=k<=n.
 * https://oeis.org/A341315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row + col) / gcd(row, col) where row and col are the *
 *            row‑index and column‑index of the flattened triangle. a(0) is
 *            defined to * be 0 (gcd(0,0) is treated specially). The function
 *            uses integer binary * search to locate the row and the GNU
 *            __int128 type to avoid overflow
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Find largest r such that r*(r+1)/2 <= n. */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Expanding hi until the inequality fails */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Row of the triangle */
    idx_t r = find_row(n);
    /* Number of terms before this row */
    idx_t prev = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    /* Column inside the row */
    idx_t k = n - prev;

    /* Special case for the single 0 element */
    if (r == 0 && k == 0)
        return 0;

    seq_t row = (seq_t)r;
    seq_t col = (seq_t)k;
    seq_t g = gcd(row, col);
    return (row + col) / g;
}
