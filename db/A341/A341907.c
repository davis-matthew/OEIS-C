/*
 * A341907  T(n, k) is the result of replacing 2^e with k^e in the binary
 *          expansion of n; square array T(n, k) read by antidiagonals
 *          upwards, n, k >= 0.
 * https://oeis.org/A341907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = T(i,j) where (i,j) is the entry obtained by reading the
 *            infinite array T(⋅,⋅) of the OEIS sequence A341907 by
 *            antidiagonals (bottom‑to‑top). For a given index n the diagonal
 *            index s satisfies T_{s-1} = s(s-1)/2 ≤ n < s(s+1)/2, and the
 *            offset within that diagonal is offset = n - s(s-1)/2. T...
 * Range:     u8 n <= 126, u16 n <= 543, u32 n <= 5605, u64 n <= 9404,
 *            u128 n <= 9404, big any n
 * Helpers:   T_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/T_term.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index s such that n lies on that diagonal.
       We search for the minimal s with s*(s+1)/2 > n.  */
    long double dbl = sqrt((long double)2 * n);
    idx_t hi = (idx_t)(dbl + 2.0L);   /* +2 to guarantee upper bound   */
    if (hi < 2) hi = 2;

    idx_t lo = 0;
    while (lo + 1 < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1);
        t /= 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t s = hi;                     /* diagonal index   */

    unsigned __int128 start128 = (unsigned __int128)s * (s - 1) / 2;
    idx_t start   = (idx_t)start128;   /* start index of this diagonal */
    idx_t offset  = n - start;         /* position inside the diagonal   */

    idx_t i = s - 1 - offset;
    idx_t j = offset;

    return T_term(i, j);
}
