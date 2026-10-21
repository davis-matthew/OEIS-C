/*
 * A281862  Riordan transform of the triangular number sequence A000217 with
 *          the Chebyshev S matrix A049310.
 * https://oeis.org/A281862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 7*a(n-3) - 6*a(n-4) + 3*a(n-5) -
 *            a(n-6) with a(0)=0, a(1)=1, a(2)=3, a(3)=4, a(4)=1, a(5)=-6
 * Range:     i8 n <= 26, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)1;
    if (n == 5) return (seq_t)-6;

    /* Roll‑through the recurrence */
    seq_t v0 = (seq_t)0;   /* a(n-6) */
    seq_t v1 = (seq_t)1;   /* a(n-5) */
    seq_t v2 = (seq_t)3;   /* a(n-4) */
    seq_t v3 = (seq_t)4;   /* a(n-3) */
    seq_t v4 = (seq_t)1;   /* a(n-2) */
    seq_t v5 = (seq_t)-6;  /* a(n-1) */

    idx_t i;
    for (i = 6; i <= n; ++i)
    {
        seq_t next = (seq_t)3 * v5
                    - (seq_t)6 * v4
                    + (seq_t)7 * v3
                    - (seq_t)6 * v2
                    + (seq_t)3 * v1
                    - v0;

        /* Shift the window */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = next;
    }
    return v5;
}
