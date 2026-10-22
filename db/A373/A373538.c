/*
 * A373538  Expansion of e.g.f. exp(x/(1 + x^2)).
 * https://oeis.org/A373538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (-1)^k * binomial(n-k-1,k)/(n-2k)!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;                 /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)    /* inclusive */
    {
        /* Compute C(n-k-1, k) multiplicatively */
        seq_t term = 1;
        for (idx_t i = 0; i < k; ++i)
        {
            seq_t num = (seq_t)(n - 2 * k + i);   /* n-2k+i */
            term *= num;
            term /= (seq_t)(i + 1);               /* divide by i+1 */
        }

        /* Multiply by product of (n-2k+1) … n (2k terms) */
        for (idx_t t = n - 2 * k + 1; t <= n; ++t)
        {
            term *= (seq_t)t;
        }

        /* Apply the sign (-1)^k */
        if (k & 1) term = -term;

        sum += term;
    }

    return sum;
}
