/*
 * A319196  a(n) = 2^(n-1)*Fibonacci(n-3), n >= 0.
 * https://oeis.org/A319196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*(a(n-1) + 2*a(n-2)), a(0) = 1, a(1) = -1
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    /* a0 and a1 hold a(n-2) and a(n-1) respectively. */
    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)-1;     /* a(1) */
    seq_t an = 0;             /* a(n) being built */

    /* Compute successive terms using the recurrence up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        an = (a1 * (seq_t)2) + (a0 * (seq_t)4); /* 2*(a1 + 2*a0) */
        a0 = a1;
        a1 = an;
    }

    return an;
}
