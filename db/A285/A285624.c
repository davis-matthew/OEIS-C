/*
 * A285624  Triangle read by rows: T(n,k) is the number of vertices with
 *          degree k counted over all hypergraphs on n labeled nodes,
 *          n>=1,0<=k<=2^(n-1).
 * https://oeis.org/A285624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = value in the triangle T(r,k) with T(r,k) = r *
 *            binomial(2^(r-1),k) * 2^(2^(r-1)-1)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A285624 (flattened triangle). */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the row `r` (1‑based) and column `c` (0‑based) for the index `n`. */
    idx_t cum = 0;          /* cumulative number of terms so far  */
    idx_t r   = 0;          /* current row number           */
    while (1) {
        r++;
        idx_t len = ((idx_t)1 << (r - 1)) + 1; /* 2^(r-1)+1 terms in row r */
        if (cum + len >= n) break;
        cum += len;
    }
    idx_t c = n - cum - 1;   /* column index in the found row */

    /* m = 2^(r-1) */
    idx_t m = ((idx_t)1 << (r - 1));

    /* factor = r * 2^(m-1) */
    seq_t factor = pow2(m - 1);      /* 2^(m-1)          */
    factor = factor * (unsigned int)r; /* multiply by r     */

    /* Compute binomial coefficient C(m,c) iteratively. */
    seq_t coeff = 1;                  /* C(m,0) = 1      */
    for (idx_t k = 0; k < c; ++k) {
        coeff = coeff * (m - k);      /* multiply by (m-k) */
        coeff = coeff / (k + 1);      /* divide by (k+1)   */
    }

    /* Result: T(r,c) = coeff * factor */
    seq_t result = coeff * factor;
    return result;
}
