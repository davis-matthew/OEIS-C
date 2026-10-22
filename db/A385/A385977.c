/*
 * A385977  Length of the long leg of the triangles defined in A377725.
 * https://oeis.org/A385977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A002315(n) * (A002315(n) + 1) where A002315(0)=1,
 *            A002315(1)=7 and for n≥2 A002315(n) = 6*A002315(n-1) –
 *            A002315(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n ≥ 0. */
seq_t a(idx_t n) {
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)4;      /* 2*1*2 */
    if (n == 1) return (seq_t)112;    /* 2*7*8 */

    /* Recurrence for A002315. */
    seq_t prevprev = (seq_t)1;   /* A002315(0) */
    seq_t prev     = (seq_t)7;   /* A002315(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)6 * prev - prevprev;
        prevprev = prev;
        prev = curr;
    }

    /* Now prev holds A002315(n). */
    seq_t bn = prev;
    seq_t result = (seq_t)2 * bn * (bn + (seq_t)1);
    return result;
}
