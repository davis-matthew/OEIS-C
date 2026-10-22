/*
 * A389173  Number of mutual-visibility sets in the n-sun graph.
 * https://oeis.org/A389173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)-6*a(n-2)+2*a(n-3)-a(n-4)+a(n-5) with initial
 *            values: a(1)=4, a(2)=13, a(3)=42, a(4)=135, a(5)=448
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)42;
    if (n == 4) return (seq_t)135;
    if (n == 5) return (seq_t)448;

    /* Variables hold the last five terms of the sequence.
       For i = 6 up to n we compute the next term using the
       given recurrence and roll the window forward. */
    seq_t a1 = 4;    /* a(n-5) */
    seq_t a2 = 13;   /* a(n-4) */
    seq_t a3 = 42;   /* a(n-3) */
    seq_t a4 = 135;  /* a(n-2) */
    seq_t a5 = 448;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        an = (seq_t)5 * a5 - (seq_t)6 * a4 + (seq_t)2 * a3 - a2 + a1;
        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
