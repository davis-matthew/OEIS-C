/*
 * A285891  Triangle read by rows: T(n,k) = n*A237048(n,k).
 * https://oeis.org/A285891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A285891: Triangle read by rows
 *
 *  Triangle:   T(n,k) = n if there exists a positive integer a such that
 *               n = a + (a+1) + ... + (a+k-1)  (i.e. a = (n - k(k-1)/2)/k is an integer ≥ 1),
 *               otherwise T(n,k) = 0.
 *
 *  The linear sequence a(n) is obtained by concatenating the rows of this triangle.
 *
 *  The function below returns the a(n) value for any n ≥ 1.
 */

#include <stdint.h>
#include "../utils/int_sqrt.h"          // int_sqrt(idx_t) returns ⌊√x⌋

seq_t a(idx_t n)
{
    if (n < 1) return 0;               // the sequence is defined for n ≥ 1

    idx_t cum      = 0;                // cumulative number of entries processed
    idx_t prev_cum = 0;
    idx_t r        = 0;                // current row number (cumulative row index)

    /* Find the row that contains the n‑th element */
    while (cum < n) {
        ++r;
        /* kmax is the largest k such that k(k+1)/2 ≤ r */
        idx_t d      = 8 * r + 1;      // d fits in 64‑bit for all intended n
        idx_t kmax   = (int_sqrt(d) - 1) / 2;
        prev_cum    = cum;
        cum        += kmax;
    }

    /* Index within the found row (1‑based) */
    idx_t k = n - prev_cum;

    /* Check whether a valid consecutive partition exists for this (r,k) */
    idx_t numerator = r - k * (k - 1) / 2;   // a = numerator / k
    if (numerator > 0 && (numerator % k == 0)) {
        return (seq_t) r;                    // T(r,k) = r
    } else {
        return 0;                            // no valid partition
    }
}
