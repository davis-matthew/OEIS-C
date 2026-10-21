/*
 * A293611  a(n) = (25*n + 41)*Pochhammer(n, 5) / 6!.
 * https://oeis.org/A293611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*n + 41)*n*(n+1)*(n+2)*(n+3)*(n+4) / 720
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 68, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /*  (25*n + 41) is computed first and stored in 'prod'.
        The product n*(n+1)*(n+2)*(n+3)*(n+4) is accumulated
        while reducing against the denominator 720 to keep
        intermediate values reasonably small.                           */
    seq_t prod = (seq_t)n * 25 + 41;
    seq_t den  = 720;

    for (idx_t i = 0; i < 5; ++i)
    {
        seq_t factor = (seq_t)(n + i);

        /* Reduce the factor with the current denominator. */
        seq_t g = seq_gcd(factor, den);
        if (g != 1)
        {
            factor /= g;
            den /= g;
        }

        prod *= factor;

        /* Reduce the running product with the (possibly reduced)
           remaining part of the denominator.                     */
        g = seq_gcd(prod, den);
        if (g != 1)
        {
            prod /= g;
            den /= g;
        }
    }

    /* In theory den should be 1 at this point; safeguard with a final division. */
    if (den != 1)
        prod /= den;

    return prod;
}
