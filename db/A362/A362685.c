/*
 * A362685  Triangle of numbers read by rows, T(n, k) = (n*(n-1))*Stirling2(k,
 *          2), for n >= 1 and 1 <= k <= n.
 * https://oeis.org/A362685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,k) = r*(r-1) * Stirling2(k,2) * where Stirling2(k,2) = 0 for
 *            k<2 and = 2^(k-1)-1 for k>=2. * a(n) is the sequence obtained by
 *            reading the triangle * T(r,k) by rows. For the given index n the
 *            function * returns the corresponding triangle entry
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n.
     * Since n can be large, use a binary search with 128-bit arithmetic
     * to avoid overflow when computing r*(r+1)/2. */
    idx_t low = 1;
    idx_t high = n;          /* upper bound is n itself */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 mid128 = (unsigned __int128)mid;
        unsigned __int128 val   = (mid128 * (mid128 + 1)) >> 1;  /* mid*(mid+1)/2 */
        if (val >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;   /* row number */

    /* Compute prefix terms before this row: r*(r-1)/2 */
    unsigned __int128 r128 = (unsigned __int128)r;
    idx_t prefix_before = (idx_t)((r128 * (r128 - 1)) >> 1);

    /* Position within the row (1‑based) */
    idx_t k = n - prefix_before;   /* 1 <= k <= r */

    /* Compute Stirling2(k,2) */
    seq_t stir;
    if (k < 2) {
        stir = (seq_t)0;
    } else {
        stir = pow2(k - 1) - (seq_t)1;   /* 2^(k-1) - 1 */
    }

    /* Compute r*(r-1) as seq_t to keep precision */
    seq_t mult = (seq_t)r * (seq_t)(r - 1);

    /* Return the triangle entry */
    return mult * stir;
}
