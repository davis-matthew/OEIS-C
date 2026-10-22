/*
 * A387849  a(n) = Sum_{k=0..n} binomial(4*n,6*k).
 * https://oeis.org/A387849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n,6*k) Recurrence for n > 5: a(n)
 *            = 6*a(n-1) + 69*a(n-2) + 1366*a(n-3) + 1359*a(n-4) + 1296*a(n-5)
 *            Initial values: a(0) = 1 a(1) = 1 a(2) = 29 a(3) = 926 a(4) =
 *            9829 a(5) = 164921
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 29;
    if (n == 3) return 926;
    if (n == 4) return 9829;
    if (n == 5) return 164921;

    /* Variables hold the last five values of the sequence. */
    seq_t a1 = 1;      /* a(n-5) */
    seq_t a2 = 29;     /* a(n-4) */
    seq_t a3 = 926;    /* a(n-3) */
    seq_t a4 = 9829;   /* a(n-2) */
    seq_t a5 = 164921; /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 6; i <= n; ++i) {
        an = 6 * a5 + 69 * a4 + 1366 * a3 + 1359 * a2 + 1296 * a1;
        /* shift the last five values */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
