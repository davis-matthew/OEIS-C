/*
 * A259476  Cayley's triangle of V numbers; triangle V(n,k), n >= 4, n <= k <=
 *          2*n-4, read by rows.
 * https://oeis.org/A259476
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term of Cayley's triangle of V numbers. Triangle
 *            entries: T(n,m) = 2*C(m,n)*C(n-2,m-n+2)/(n-2) , n>=4, with m
 *            ranging from n to 2n-4. The OEIS sequence lists the triangle row
 *            by row, starting at n=4
 * Range:     u8 n <= 11, u16 n <= 34, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below builds the value directly from (n,m) using
   the binomial helper in utils/binom.h.  It works for any n>=4,
   in any order of calls, and without hard‑coded tables.  Only
   integer arithmetic is used. */

#include <stdint.h>
#include "../utils/binom.h"

/* Find the row number r (>=4) such that (r-3)(r-2)/2 > idx ,
   where idx = n-4 is the zero‑based position in the triangular
   traversal.  Binary search is used on r. */
static idx_t find_row(idx_t idx)
{
    idx_t low  = 4;                     /* smallest possible row */
    idx_t high = idx * 2 + 10;          /* safe upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 cum = (__int128)(mid - 3) * (mid - 2) / 2;
        if (cum > idx)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;                         /* minimal r with cum > idx */
}

seq_t a(idx_t n)
{
    if (n < 4)                /* sequence undefined below offset */
        return 0;

    idx_t idx = n - 4;        /* zero‑based index in the triangle */
    idx_t r   = find_row(idx);    /* current row number (n) */

    /* Cumulative count of terms up to row r-1: (r-4)(r-3)/2 */
    __int128 cum_prev = (__int128)(r - 4) * (r - 3) / 2;
    idx_t    offset   = idx - (idx_t)cum_prev;   /* column offset */
    idx_t    k        = r + offset;              /* column index (m) */

    /* Compute the formula: 2 * C(k,r) * C(r-2, k-r+2) / (r-2) */
    seq_t c1 = binom(k, r);                       /* C(k, r)   */
    seq_t c2 = binom(r - 2, k - r + 2);           /* C(r-2, k-r+2) */

    seq_t val = c1;
    val = val * c2;                               /*  C(k,r)*C(r-2,k-r+2) */
    val = val * 2;                                /*  *2                 */
    val = val / (r - 2);                          /*  divide by n-2 */

    return val;
}
