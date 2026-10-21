/*
 * A262742  Irregular table read by rows: T(n,k) is the number of binary
 *          symmetric n X n matrices with exactly k 1's; n>=0, 0<=k<=n^2.
 *          Where the symmetry axes are in horizontal and vertical.
 * https://oeis.org/A262742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A262742
    Irregular table read by rows, counting binary matrices symmetric with
    respect to the horizontal and vertical axes.

    For a given row r the number of independent cells is
        a = (r even) ? (r/2)^2
                     : ((⌈r/2⌉-1)^2);          // weight 4 cells
        b = (r odd)  ? 2*(⌈r/2⌉-1) : 0;      // weight 2 cells
        c = (r odd)  ? 1        : 0;          // weight 1  cell

    The value at column k equals the coefficient of x^k in
        (1+x^4)^a · (1+x^2)^b · (1+x)^c .
    The function a(n) receives the linear index n and returns this
    coefficient using a simple O(a·b) loop.  No hard‑coded table is used.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to the linear
       index n.  The i‑th row has length i^2+1.  We scan rows
       until the cumulative length exceeds n. */
    idx_t r = 0;
    idx_t cum = 0;            /* cumulative length of all previous rows */
    for (;;) {
        idx_t row_len = r * r + 1;
        if (cum + row_len > n) break;
        cum += row_len;
        ++r;
    }
    idx_t k = n - cum;        /* position within row r (0‑based) */

    /* Compute the counts of independent cells for row r.  */
    idx_t m = (r + 1) / 2;                   /* ceil(r/2) */
    idx_t a_cells, b_cells, c_cell;
    if ((r & 1) == 0) {                     /* r even */
        a_cells = m * m;
        b_cells = 0;
        c_cell  = 0;
    } else {                                 /* r odd */
        a_cells = (m - 1) * (m - 1);
        b_cells = 2 * (m - 1);
        c_cell  = 1;
    }

    /* Pre‑compute binomial coefficients for a_cells and b_cells.
       bin_a[p] = C(a_cells,p).  Use the multiplicative recurrence
       to avoid factorials.  */
    size_t a_size = (size_t)a_cells + 1;
    seq_t *bin_a = (seq_t *)malloc(a_size * sizeof(seq_t));
    if (!bin_a) return 0;
    bin_a[0] = 1;
    for (idx_t p = 1; p <= a_cells; ++p) {
        bin_a[p] = bin_a[p - 1] * (seq_t)(a_cells - (p - 1))
                                / (seq_t)p;
    }

    size_t b_size = (size_t)b_cells + 1;
    seq_t *bin_b = (seq_t *)malloc(b_size * sizeof(seq_t));
    if (!bin_b) { free(bin_a); return 0; }
    bin_b[0] = 1;
    for (idx_t q = 1; q <= b_cells; ++q) {
        bin_b[q] = bin_b[q - 1] * (seq_t)(b_cells - (q - 1))
                                / (seq_t)q;
    }

    /* Enumerate p (weight‑4 cells) and q (weight‑2 cells) and
       check whether the remaining weight k‑4p‑2q equals 0 or 1. */
    seq_t result = 0;
    for (idx_t p = 0; p <= a_cells; ++p) {
        seq_t sum_p = (seq_t)4 * (seq_t)p;
        if (sum_p > (seq_t)k) break;            /* early quit */
        seq_t term_a = bin_a[p];
        for (idx_t q = 0; q <= b_cells; ++q) {
            seq_t sum_q = (seq_t)2 * (seq_t)q;
            idx_t weight = (idx_t)(sum_p + sum_q);
            if (weight > k) break;
            idx_t rem = k - weight;
            if (rem > c_cell) continue;
            /* binom(c_cell,rem) is 1 for rem=0 or rem=1 */
            result += term_a * bin_b[q];
        }
    }

    free(bin_a);
    free(bin_b);
    return result;
}
