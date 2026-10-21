/*
 * A291033  p-INVERT of the positive integers, where p(S) = 1 - 6*S.
 * https://oeis.org/A291033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-2), with a(0)=6, a(1)=48
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the two known starting values explicitly. */
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)48;

    /* Use the recurrence to generate terms up to n. */
    seq_t a_prev = (seq_t)6;   /* a(0) */
    seq_t a_curr = (seq_t)48;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_next = 8 * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
