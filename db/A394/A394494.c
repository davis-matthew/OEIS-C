/*
 * A394494  Expansion of e.g.f. (1/x) * Series_Reversion( x*(1 - 4*x)^(1/4) ).
 * https://oeis.org/A394494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=0}^{n-1} (5*n - 3 - 4*k) / (n + 1) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the base case explicitly */
    if (n == 0) return (seq_t)1;

    /* Numerator accumulates the product, denominator starts as n+1 */
    seq_t num = 1;
    seq_t den = (seq_t)(n + 1);

    for (idx_t k = 0; k < n; ++k) {
        /* Compute the current factor: 5*n - 3 - 4*k  (always positive) */
        seq_t term = (seq_t)(5 * n - 3 - 4 * k);
        num *= term;

        /* Reduce with the denominator whenever possible to keep numbers small */
        seq_t g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }
    }

    /* After the loop the denominator should have been cancelled entirely */
    return num;
}
