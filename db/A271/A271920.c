/*
 * A271920  Denominator of Product_{j=1..n-1} ((3*j+1)/(3*j+2)).
 * https://oeis.org/A271920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of product_{j=1..n-1} (3*j+1)/(3*j+2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n = 1 the product is empty, so the denominator is 1. */
    if (n <= 1) return (seq_t)1;

    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (idx_t j = (idx_t)1; j < n; ++j) {
        seq_t nf = (seq_t)(3*j + 1);   /* factor for the numerator   */
        seq_t df = (seq_t)(3*j + 2);   /* factor for the denominator */

        num *= nf;
        den *= df;

        /* Cancel common factors immediately to keep the values small. */
        seq_t g = gcd(num, den);
        if (g > (seq_t)1) {
            num /= g;
            den /= g;
        }
    }

    return den;          /* the reduced denominator after all cancellations */
}
