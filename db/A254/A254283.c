/*
 * A254283  Indices of hexagonal numbers (A000384) which are also centered
 *          triangular numbers (A005448).
 * https://oeis.org/A254283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 194*a(n-2) - 194*a(n-3) - a(n-4) + a(n-5) a(1) =
 *            1, a(2) = 31, a(3) = 115, a(4) = 5965, a(5) = 22261
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)31;
    if (n == 3) return (seq_t)115;
    if (n == 4) return (seq_t)5965;
    if (n == 5) return (seq_t)22261;

    /* t1 == a(i-1), t2 == a(i-2), ..., t5 == a(i-5) */
    seq_t t1 = (seq_t)22261; /* a(5)  */
    seq_t t2 = (seq_t)5965;  /* a(4)  */
    seq_t t3 = (seq_t)115;   /* a(3)  */
    seq_t t4 = (seq_t)31;    /* a(2)  */
    seq_t t5 = (seq_t)1;     /* a(1)  */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = t1;
        val += (seq_t)194 * t2;
        val -= (seq_t)194 * t3;
        val -= t4;
        val += t5;

        /* shift window */
        t5 = t4;
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = val;
    }
    return t1;
}
