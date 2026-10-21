/*
 * A253458  Indices of centered heptagonal numbers (A069099) which are also
 *          centered hexagonal numbers (A003215).
 * https://oeis.org/A253458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 27*a(n-2) + a(n-3), with a(1)=1, a(2)=13,
 *            a(3)=325
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A253458 by direct recurrence. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset < 1, handled gracefully */

    /* Base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)325;

    /* Iterate recurrence from 4 up to n */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)13;  /* a(2) */
    seq_t a3 = (seq_t)325; /* a(3) */
    seq_t ai = a3;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)27 * a3 - (seq_t)27 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
