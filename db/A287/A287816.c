/*
 * A287816  Number of nonary sequences of length n such that no two
 *          consecutive terms have distance 1.
 * https://oeis.org/A287816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 11*a(n-2) - 15*a(n-3) + 19*a(n-4) + a(n-5)
 *            Initial terms: a(0)=1, a(1)=9, a(2)=65, a(3)=471, a(4)=3413
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)65;
    if (n == 3) return (seq_t)471;
    if (n == 4) return (seq_t)3413;

    seq_t a0 = (seq_t)1;    /* a(i-5) */
    seq_t a1 = (seq_t)9;    /* a(i-4) */
    seq_t a2 = (seq_t)65;   /* a(i-3) */
    seq_t a3 = (seq_t)471;  /* a(i-2) */
    seq_t a4 = (seq_t)3413; /* a(i-1) */
    seq_t an = 0;

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)9 * a4 - (seq_t)11 * a3 - (seq_t)15 * a2
             + (seq_t)19 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
