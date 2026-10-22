/*
 * A367338  Comma-successor to n: second term of commas sequence if initial
 *          term is n, or -1 if there is no second term.
 * https://oeis.org/A367338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = smallest m > n such that m – n = 10*lsd(n) + msd(m),
 *            otherwise -1. lsd(n) = least significant decimal digit of n,
 *            msd(m) = most significant decimal digit of m
 * Range:     i8 n <= 38, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;          /* convert index to seq_t */
    seq_t x = n_val % 10;            /* least significant digit of n */

    for (seq_t y = 1; y <= 9; ++y) { /* try each possible leading digit of m */
        seq_t m = n_val + 10 * x + y;
        seq_t t = m;
        while (t >= 10)               /* find most significant digit of m */
            t /= 10;
        if (t == y)                  /* check the defining condition */
            return m;
    }
    return (seq_t)-1;                /* no child exists */
}
