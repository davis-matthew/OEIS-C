/*
 * A300296  Numerators of n*(5 + 3*n)/(8*(1 + 3*n)*(4 + 3*n)), n >= 0.
 * https://oeis.org/A300296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(n*(5+3*n)/(8*(1+3*n)*(4+3*n)))
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t tn = (seq_t)n;
    /* numerator: n*(5 + 3*n) */
    seq_t num = tn * (((seq_t)3 * tn) + (seq_t)5);
    /* denominator: 8*(1 + 3*n)*(4 + 3*n) */
    seq_t den = ((seq_t)8)
                * (((seq_t)3 * tn) + (seq_t)1)
                * (((seq_t)3 * tn) + (seq_t)4);
    seq_t g = gcd(num, den);
    return num / g;
}
