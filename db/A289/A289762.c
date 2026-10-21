/*
 * A289762  Triangular array T(m,k) = (m+1-k)^2 + k - 1 with m (row) >= 1 and
 *          k (column) >= 1, read by rows.
 * https://oeis.org/A289762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 240, u16 n <= 306, u32 n <= 306, u64 n <= 306,
 *            u128 n <= 306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A289762:  T(m,k) = (m+1-k)^2 + k - 1  (read by rows)
 *  a(1)=1, a(2)=1, a(3)=4, a(4)=2, …  (as given in the OEIS entry)
 *
 *  The index n is 1‑based.  Row m contains 2m numbers and the
 *  cumulative count up to row m is m(m+1).  For a given n we
 *  find the smallest m with m(m+1) ≥ n, then k = n – m(m−1)
 *  (the column in that row).  Finally compute T(m,k).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset 1: a(1) is the first element */
    if (n == 0) return (seq_t)0;   /* undefined for n=0 – return 0 */

    /* find smallest m such that m*(m+1) >= n */
    idx_t m = 1;
    for (;;) {
        /* use unsigned __int128 to avoid overflow for very large n */
        unsigned __int128 prod = (unsigned __int128)m * (m + 1);
        if ((unsigned __int128)n <= prod)
            break;
        ++m;
    }

    /* elements before this row: (m-1)*m */
    idx_t prev = (m - 1) * m;
    /* 1‑based column in the row */
    idx_t k = n - prev;

    /* compute (m+1-k)^2 + k - 1.
     * Use signed arithmetic for the subtraction to avoid
     * unsigned underflow when k > m+1. */
    int64_t diff   = (int64_t)(m + 1) - (int64_t)k;   /* may be negative */
    int64_t sq     = diff * diff;                      /* always non‑negative */
    seq_t term     = (seq_t)sq + (seq_t)k - (seq_t)1;  /* cast to seq_t at end */

    return term;
}
