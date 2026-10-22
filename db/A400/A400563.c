/*
 * A400563  Irregular triangle read by rows: Row n >= 2 lists numbers 1 <= k
 *          <= 2^n such that chi_{2^n,k}(k) = -1, where chi_{N,k} is the
 *          Dirichlet character modulo N with label k in the Conrey labeling
 *          system.
 * https://oeis.org/A400563
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy, new
 * Formula:   a(n) = 2^r * k - (-1)^(row*k) with r = floor(row/2)+1. The
 *            sequence is read row by row; row(n) and position k(n) are found
 *            by subtracting row lengths from the index offset, then the term
 *            is evaluated by the OEIS formula
 * Range:     u8 n <= 38, u16 n <= 638, u32 n <= 9492, u64 n <= 9492,
 *            u128 n <= 9492, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence starts at index 2.  For n < 2 the value is undefined,
       but we return 0 to avoid undefined behaviour. */
    if (n < 2) return 0U;

    /* offset 0 for the first term (a(2)=3) */
    idx_t pos = n - 2;
    idx_t row = 2;

    for (;;) {
        /* r = floor(row/2) + 1 */
        idx_t r   = row / 2 + 1;
        /* length of this row = 2^(row-r) */
        seq_t len = pow2(row - r);

        if ((seq_t)pos < len) break;     /* the term is in this row */

        pos -= (idx_t)len;                /* skip this entire row */
        ++row;
    }

    /* k is the 1‑based position within the row */
    seq_t k   = (seq_t)(pos + 1);
    seq_t pow = pow2(row / 2 + 1);        /* 2^r */

    seq_t term = pow * k;

    /* subtract (-1)^(row*k) */
    if ((row & 1) == 0) {              /* row even → (-1)^(even)=+1 */
        term -= (seq_t)1;
    } else {                           /* row odd → sign depends on k */
        if ((k & (seq_t)1) == 1)      /* k odd → (-1)^(odd) = -1 → -(-1)=+1 */
            term += (seq_t)1;
        else                          /* k even → (-1)^(even)=+1 → -1 */
            term -= (seq_t)1;
    }

    return term;
}
