/*
 * A300295  Denominator of (1/3)*n*(n + 2)/((1 + 2*n)*(3 + 2*n)).
 * https://oeis.org/A300295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of (1/3)*n*(n+2)/((1+2n)*(3+2n))
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n = 0 we explicitly return 1, because the numerator is 0. */
    if (n == 0)
        return 1;

    /* Compute numerator = n*(n+2)   */
    seq_t num = (seq_t)n * ((seq_t)n + 2);

    /* Compute denominator = 3*(1+2n)*(3+2n)   */
    seq_t den = 3 * ((seq_t)1 + 2 * (seq_t)n) * ((seq_t)3 + 2 * (seq_t)n);

    /* Reduce the fraction and return the denominator after reduction */
    seq_t g = gcd(num, den);
    return den / g;
}
