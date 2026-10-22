/*
 * A394978  Expansion of -1/(Sum_{k>=0} binomial(3*k+1,k) * x^k).
 * https://oeis.org/A394978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1; a(1) = 4; for n >= 1: a(n+1) = a(n) * 3*(3n-2)*(3n+2)
 *            / ( 2*(n+1)*(2n+1) )
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 26, i128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)-1;

    seq_t res = (seq_t)4;   /* a(1) */

    for (idx_t i = 1; i < n; ++i)
    {
        /* numerator = 3*(3*i-2)*(3*i+2) */
        seq_t num = (seq_t)3 * (seq_t)(3*i - 2) * (seq_t)(3*i + 2);
        /* denominator = 2*(i+1)*(2*i+1) */
        seq_t denom = (seq_t)2 * (seq_t)(i + 1) * (seq_t)(2*i + 1);

        /* Reduce res with denominator */
        seq_t g = gcd(res, denom);
        if (g != (seq_t)0)
        {
            res   /= g;
            denom /= g;
        }

        /* Reduce numerator with remaining denominator */
        g = gcd(num, denom);
        if (g != (seq_t)0)
        {
            num   /= g;
            denom /= g;
        }

        /* In theory denom should now be 1; multiply */
        res = res * num;
        /* If denom is somehow >1 (shouldn't happen), divide */
        if (denom != (seq_t)1)
            res /= denom;
    }

    return res;
}
