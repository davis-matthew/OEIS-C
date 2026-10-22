/*
 * A339236  Irregular triangle of incomplete Leonardo numbers read by rows.
 *          T(n, k) = 2*(Sum_{j=0..k} binomial(n-j, j)) - 1, for n>=0 and
 *          0<=k<=floor(n/2).
 * https://oeis.org/A339236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2 * (Σ_{j=0..k} binomial(r - j, j)) - 1 * where r is the
 *            row index such that the cumulative number of terms * up to row
 *            r-1 is ≤ n < cumulative up to row r, and * k = n -
 *            cumulative_up_to_row(r‑1). The triangle is read row‑wise *
 *            (inner loop over k). binomial(n, k) is returned as a seq_t....
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n */
    idx_t r   = 0;          /* current row index (starting from 0)   */
    idx_t cum = 0;          /* number of terms before this row      */
    while (1) {
        /* length of current row = floor(r/2) + 1 */
        idx_t len = (r >> 1) + 1;
        if (cum + len > n)        /* n lies in this row           */
            break;
        cum += len;          /* move to next row              */
        ++r;
    }

    /* k is the position within row r */
    idx_t k = n - cum;

    /* Compute the inner sum Σ_{j=0..k} binomial(r - j, j) */
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        sum = sum + binomial(r - j, j);
    }

    /* Apply the outer formula */
    seq_t val = sum * 2 - 1;
    return val;
}
