/*
 * A290991  p-INVERT of (0,0,1,2,3,4,5,...), the nonnegative integers A000027
 *          preceded by one zero, where p(S) = 1 - S - S^2.
 * https://oeis.org/A290991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 3*a(n-4) + a(n-5) +
 *            a(n-6) with a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=6,
 *            a(6)=13
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 70, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    seq_t a1 = (seq_t)0;   /* a(0) */
    seq_t a2 = (seq_t)0;   /* a(1) */
    seq_t a3 = (seq_t)1;   /* a(2) */
    seq_t a4 = (seq_t)2;   /* a(3) */
    seq_t a5 = (seq_t)3;   /* a(4) */
    seq_t a6 = (seq_t)6;   /* a(5) */
    seq_t a7 = (seq_t)13;  /* a(6) */

    if (n == 0) return a1;
    if (n == 1) return a2;
    if (n == 2) return a3;
    if (n == 3) return a4;
    if (n == 4) return a5;
    if (n == 5) return a6;
    if (n == 6) return a7;

    /* For n >= 7, iterate using the recurrence */
    seq_t ai;
    for (idx_t i = 7; i <= n; ++i) {
        ai = (seq_t)4 * a7
            - (seq_t)6 * a6
            + (seq_t)5 * a5
            - (seq_t)3 * a4
            + a3
            + a2;

        /* Shift the window of previous terms */
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = ai;
    }
    return ai;
}
