/*
 * A264389  Denominator of binomial(n-1, 2)/(6*n), for n >= 1. Denominator of
 *          Dedekind sum s(1,n).
 * https://oeis.org/A264389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(binomial(n-1, 2)/(6*n))
 * Range:     u8 n <= 18, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) {
        /* For n = 1 or 2 the numerator is 0, so the reduced fraction is 0/1 */
        return (seq_t)1;
    }

    seq_t num = (seq_t)(n - 1) * (n - 2);   /* (n-1)(n-2) */
    seq_t den = (seq_t)12 * n;              /* 12*n */

    seq_t g   = gcd(num, den);              /* gcd of numerator and denominator  */
    return den / g;                         /* reduced denominator */
}
