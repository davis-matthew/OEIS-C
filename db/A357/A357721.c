/*
 * A357721  a(n) = Sum_{k=0..floor(n/2)} (-n)^k * Stirling1(n,2*k).
 * https://oeis.org/A357721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..floor(n/2)} (-n)^k * Stirling1(n,2k).
 * Implements the definition directly; O(n²) time, O(n) memory.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for the Stirling numbers of the first kind (signed). */
    seq_t *st = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!st) return 0;   /* unexpected out‑of‑memory */

    /* s(0,0) = 1, all other entries are initialized to 0 */
    st[0] = 1;

    /* Build the Stirling numbers of the first kind row by row. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t k = i; k > 0; --k) {
            st[k] = st[k - 1] - (seq_t)(i - 1) * st[k];
        }
        st[0] = 0;       /* s(i,0) = 0 for i > 0 */
    }

    /* Sum over even indices with powers of (-n). */
    seq_t sum   = 0;
    seq_t coeff = 1;                     /* (-n)^0 */
    seq_t neg_n = -(seq_t)n;             /* -n in seq_t */

    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        sum += coeff * st[2 * k];
        coeff *= neg_n;                   /* multiply by (-n) for next term */
    }

    free(st);
    return sum;
}
