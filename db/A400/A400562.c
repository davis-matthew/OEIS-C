/*
 * A400562  Irregular triangle read by rows: Row n >= 2 lists numbers 1 <= k
 *          <= 2^n such that chi_{2^n,k}(k) = 1, where chi_{N,k} is the
 *          Dirichlet character modulo N with label k in the Conrey labeling
 *          system.
 * https://oeis.org/A400562
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 638, u32 n <= 8400, u64 n <= 8400,
 *            u128 n <= 8400, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A400562: irregular triangle read by rows
   Formula: a(n) = T(m,k) where n = 2 + sum_{i=2}^{m-1} 2^(i - floor(i/2)-1) + (k-1),
   T(m,k) = 2^r*(k-1) + (-1)^(m*(k-1)),
   r = floor(m/2)+1
   m is the row number, k the position within that row. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* below the offset 2 the sequence is undefined – return 0 */
    if (n < 2)
        return (seq_t)0;

    /* zero‑based index of the term in the concatenated rows */
    idx_t idx = n - 2;

    /* find the row m containing this term */
    idx_t m = 2;
    while (1) {
        idx_t r = (m >> 1) + 1;            /* r = floor(m/2)+1 */
        idx_t len_pow = m - r;             /* exponent for 2^len */
        idx_t len = ((idx_t)1) << len_pow; /* number of terms in row m */

        if (idx < len)
            break;                         /* term lies in this row */

        idx -= len;                         /* skip this whole row */
        ++m;
    }

    /* position k within row m (1‑based) */
    idx_t k = idx + 1;
    seq_t base = pow2(((m >> 1) + 1));

    /* compute the term using the formula */
    seq_t term = base * (seq_t)(k - 1);
    if ((m & 1) == 0) {                 /* even row: add +1 */
        term += (seq_t)1;
    } else {                             /* odd row: add (-1)^(k-1) */
        if ((k - 1) & 1)                 /* k-1 odd -> subtract 1 */
            term -= (seq_t)1;
        else                             /* k-1 even -> add 1 */
            term += (seq_t)1;
    }

    return term;
}
