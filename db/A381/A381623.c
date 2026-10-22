/*
 * A381623  Triangle read by rows: T(n,k) is the number of the k-th eliminated
 *          person in the variation of the Josephus elimination process for n
 *          people, where the first person is eliminated, then two people are
 *          skipped, and then the process repeats.
 * https://oeis.org/A381623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   kth_elimination
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: T(n,k) = k-th eliminated person when n people are in a circle,
 *     first person is eliminated, then two people are skipped, and the
 *     process repeats.
 * a(n) is the flattened triangle read by rows: a(1)=T(1,1), a(2)=T(2,1),
 *     a(3)=T(2,2), a(4)=T(3,1), ...
 *
 * Compute a(n) by determining the row r such that r(r+1)/2 ≥ n, then
 * finding the k‑th element of that row (0‑based) using an O(r) linked‑list
 * elimination simulation.  No floating point arithmetic is used.
 */

#include <stdlib.h>
#include "../utils/kth_elimination.h"

/*  Helper to find the row number r for a given flattened index n  */
static idx_t
row_for_index(idx_t n)
{
    /* Find smallest r with r(r+1)/2 ≥ n using binary search. */
    idx_t lo = 0, hi = 1;
    while (( (__int128)hi * (hi + 1) / 2 ) < n)
        hi <<= 1;
    while (lo + 1 < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ( (( __int128)mid * (mid + 1) / 2 ) >= n)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

/*  Main sequence function required by the build.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* unused, offset starts at 1 */

    idx_t r = row_for_index(n);                 /* row number */
    idx_t prev_tri = (r - 1) * r / 2;           /* elements before this row */
    idx_t k = n - prev_tri - 1;                 /* zero‑based index in row */

    return kth_elimination(r, k);
}
