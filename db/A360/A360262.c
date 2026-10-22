/*
 * A360262  Determinant of the pentadiagonal symmetric nXn Toeplitz Matrix
 *          with a=b=1, c=3.
 * https://oeis.org/A360262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 54*a(n-4) + 243*a(n-5)
 *            with initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=-4, a(4)=56
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 21, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)-4;
    if (n == 4) return (seq_t)56;

    /* Circular buffer for the last five terms */
    seq_t a0 = (seq_t)1;   /* a(n-5)   */
    seq_t a1 = (seq_t)1;   /* a(n-4)   */
    seq_t a2 = (seq_t)0;   /* a(n-3)   */
    seq_t a3 = (seq_t)-4;  /* a(n-2)   */
    seq_t a4 = (seq_t)56;  /* a(n-1)   */
    seq_t ai;              /* current a(n) */

    for (idx_t i = 5; i <= n; ++i)
    {
        ai = - (seq_t)2 * a4
              + (seq_t)2 * a3
              - (seq_t)6 * a2
              + (seq_t)54 * a1
              + (seq_t)243 * a0;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return ai;
}
