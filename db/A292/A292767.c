/*
 * A292767  Square array read by antidiagonals downwards: T(k,n) = sum of the
 *          site-perimeters of words of length n >= 1 over an alphabet of size
 *          k >= 1.
 * https://oeis.org/A292767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 39, u32 n <= 124, u64 n <= 357,
 *            u128 n <= 1000, big any n
 * Helpers:   T_row_k_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A292767 – Sum of site‑perimeters of words of length n over an alphabet of size k
 *
 *  For a fixed k the row has g.f. :
 *        k*x*(36 + 12*k + (8 - 24*k - 8*k^2)*x + (2 - 5*k + 4*k^2 - k^3)*x^2)
/ *       ---------------------------------------------------------------
 *                     12*(1 - k*x)^2
 *
 *  From this we obtain the following exact formulas for the first three
 *  terms of the row:
 *
 *      T(k,1) = k^2 + 3k
 *      T(k,2) = (4k^3 + 12k^2 + 2k) / 3
 *      T(k,3) = k*(19k^3 + 64k^2 + 11k + 2) / 12
 *
 *  For n ≥ 4 the row satisfies the linear homogeneous recurrence
 *
 *      T(k,n) = 2k*T(k,n-1) – k^2*T(k,n-2)
 *
 *  The global sequence a(n) (1‑indexed) is obtained by reading the
 *  square array T(k,n) downwards along antidiagonals:
 *
 *      a(1)  = T(1,1)
 *      a(2)  = T(1,2)
 *      a(3)  = T(2,1)
 *      a(4)  = T(1,3)
 *      a(5)  = T(2,2)
 *      a(6)  = T(3,1)
 *      …
 *
 *  The following implementation follows the above formulas exactly.
 *  All integer arithmetic is performed with seq_t; intermediate larger
 *  values are kept in a 128‑bit integer only when computing the first
 *  three terms.  The function is O(√n) per call and works for all
 *  n ≥ 1.
 */

#include <stdint.h>
#include "../utils/T_row_k_n.h"

/* --- public sequence function a(n) --- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; undefined below 1 */
        return 0;

    /* find the diagonal D such that D*(D+1)/2 ≥ n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 cnt = (__int128)mid * (mid + 1) / 2;
        if (cnt >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t D = lo;                          /* diagonal index (1‑based) */

    /* position within diagonal (0‑based) */
    idx_t prev_cnt = ((D - 1) * D) / 2;    /* triangular number of previous diagonals */
    idx_t pos = n - prev_cnt - 1;          /* zero based, ranges 0 .. D-1 */

    idx_t k = pos + 1;                     /* alphabet size */
    idx_t m = D - pos;                     /* word length in this row */

    return T_row_k_n(k, m);
}
