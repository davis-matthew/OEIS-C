/*
 * A264388  Numerators of binomial(n-1, 2)/(6*n), for n >= 1. Numerators of
 *          Dedekind sum s(1, n).
 * https://oeis.org/A264388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(binomial(n-1,2)/(6*n)) = (n-1)(n-2) / (12*n)
 *            reduced to lowest terms
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    seq_t num = (seq_t)(n - 1) * (seq_t)(n - 2);   /* (n-1)(n-2) */
    seq_t den = (seq_t)12 * (seq_t)n;              /* 12*n */

    seq_t g = gcd(num, den);
    return num / g;
}
