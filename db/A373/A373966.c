/*
 * A373966  Triangle read by rows: T(n,k) = (-1)^(n+1) * A000166(n) + (-1)^(k)
 *          * A000166(k) for n >= 2 and 1 <= k <= n-1.
 * https://oeis.org/A373966
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 29, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   derangement
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS formula:  a(n) = T(nrow, k)
   where  T(n,k) = (-1)^(n+1) * !n + (-1)^k * !k   (n ≥ 2, 1 ≤ k ≤ n-1)
   and a(2) is the first term of the triangle read by rows.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/derangement.h"

/* Integer square root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid == 0) { lo = 1; continue; }
        if (mid <= x / mid) { lo = mid + 1; }
        else { hi = mid - 1; }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* the sequence is defined for n ≥ 2 */

    /* row and column indices in the triangular array */
    idx_t f = n - 2;               /* offset from the first element (index 2) */

    /* Find the row r that contains the element with offset f.
       r is the smallest integer ≥ 2 such that r*(r-1)/2 > f.  */
    idx_t t = 1 + 8 * f;
    idx_t s = isqrt_u64(t);        /* floor(sqrt(8f+1))          */
    idx_t r0 = (1 + s) / 2;        /* candidate row              */
    idx_t cum_before = r0 * (r0 - 1) / 2;
    if (cum_before <= f) r0++;     /* adjust when f lies on the start of a row */
    idx_t r = r0;

    /* Column index k (1‑based within the row) */
    idx_t prior_cum = (r - 1) * (r - 2) / 2;
    idx_t k = f - prior_cum + 1;   /* 1 ≤ k ≤ r-1 */

    /* Compute the required derangements and combine them with signs */
    seq_t term_r = derangement(r);
    if ((r + 1) & 1) term_r = -term_r;   /* (-1)^(r+1) */
    seq_t term_k = derangement(k);
    if (k & 1) term_k = -term_k;         /* (-1)^k     */

    return term_r + term_k;
}
