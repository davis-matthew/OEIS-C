/*
 * A275506  Number of 6 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2) + 4*a(n-3) - 4*a(n-4) for n > 5
 *            Initial values: a(1)=24, a(2)=104, a(3)=516, a(4)=2628,
 *            a(5)=13384 (a(n) is defined for n≥1, behaviour for n<1 is
 *            undefined)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Base cases */
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)104;
    if (n == 3) return (seq_t)516;
    if (n == 4) return (seq_t)2628;
    if (n == 5) return (seq_t)13384;

    /* Keep the last four values: a(n-4) … a(n-1) */
    seq_t a_n4 = (seq_t)24;      /* a(1) */
    seq_t a_n3 = (seq_t)104;     /* a(2) */
    seq_t a_n2 = (seq_t)516;     /* a(3) */
    seq_t a_n1 = (seq_t)2628;    /* a(4) */
    seq_t a_n  = (seq_t)13384;   /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)3 * a_n   +   /* 3*a(n-1) */
                     (seq_t)10 * a_n1  +   /* 10*a(n-2) */
                     (seq_t)4 * a_n2  -   /* 4*a(n-3) */
                     (seq_t)4 * a_n3;      /* -4*a(n-4) */
        a_n3 = a_n2;   /* shift for next iteration */
        a_n2 = a_n1;
        a_n1 = a_n;
        a_n  = next;
    }
    return a_n;
}
