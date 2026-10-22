/*
 * A390989  Triangle read by rows where T(n,k) is the number of integer
 *          partitions of n with first sums summing to k+n-1, where 0 <= k <
 *          n.
 * https://oeis.org/A390989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle read by rows. The
 *            triangle entry T(r,k) counts integer partitions of r whose
 *            largest part plus smallest part equals r‑k+1. The indices start
 *            at 1 (so a(1) is the first term)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   _partitions_in_range
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_partitions_in_range.h"

seq_t a(idx_t n)
{
    /* find the row number r such that  r*(r+1)/2 ≥ n   */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* position of the term inside its row (0‑based) */
    idx_t idx_in_row = n - r * (r - 1) / 2 - 1;
    idx_t k = idx_in_row;               /* 0 ≤ k < r */

    /* special case k == 0 */
    if (k == 0) {
        return (r == 1) ? (seq_t)1 : (seq_t)0;
    }

    /* remaining parameters */
    idx_t s = r - k + 1;                /* sum of largest and smallest parts */
    idx_t m = k - 1;                    /* m = k-1 as derived */

    seq_t total = 0;
    for (idx_t a = 1; a <= s / 2; ++a) {
        idx_t b = s - a;
        total += _partitions_in_range(m, a, b);
    }
    return total;
}
