/*
 * A273630  a(n) = Sum_{k = 0..n} (-1)^k*k^3*binomial(n,k)^3.
 * https://oeis.org/A273630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * k^3 * binomial(n,k)^3. The sequence
 *            satisfies a(2*n) = 0 and for odd indices a(2*n+1) = (-1)^(n+1) *
 *            (2*n+1)^3 * (3*n)! / (n!)^3
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n = 0 gives the first term 0. */
    if (n == 0)
        return (seq_t)0;

    /* Even indices are zero. */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* n is odd. Write n = 2*m+1. */
    idx_t m = (n - 1) >> 1;

    /* Compute (2*m+1)^3. */
    seq_t twop = (seq_t)(2 * m + 1);          /* 2*m+1 */
    seq_t term = twop * twop * twop;          /* (2*m+1)^3 */

    /* Multiply by the multinomial (3*m)!/(m!)^3,
       which equals binom(3*m, m) * binom(2*m, m). */
    seq_t b1 = binom(3 * m, m);               /* binom(3*m, m) */
    seq_t b2 = binom(2 * m, m);               /* binom(2*m, m) */
    term *= b1;
    term *= b2;

    /* Apply the sign (-1)^(m+1).  When m is even we have a negative sign. */
    if ((m & 1ULL) == 0)
        term = -term;

    return term;
}
