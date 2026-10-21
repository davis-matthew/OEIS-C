/*
 * A264080  a(n) = 6*F(n)*F(n+1) + (-1)^n, where F = A000045.
 * https://oeis.org/A264080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*F(n)*F(n+1) + (-1)^n (implemented using the linear
 *            recurrence) a(0) = 1, a(1) = 5, a(2) = 13 a(n) = 2*a(n-1) +
 *            2*a(n-2) - a(n-3) for all n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first three values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)13;

    /* Iteratively compute up to a(n) using the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)5;   /* a(n-2) */
    seq_t a2 = (seq_t)13;  /* a(n-1) */
    seq_t an = 0;          /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        an = a2;          /* a(n-1) */
        an *= (seq_t)2;   /* 2*a(n-1) */
        seq_t tmp = a1;
        tmp *= (seq_t)2;  /* 2*a(n-2) */
        an += tmp;        /* +2*a(n-2) */
        an -= a0;         /* -a(n-3) */
        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}
