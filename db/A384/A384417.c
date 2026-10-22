/*
 * A384417  Expansion of g.f. cosh(9*arctanh(8*sqrt(x))).
 * https://oeis.org/A384417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16^n*(105 + 400*n + 3392*n^2 + 512*n^3 +
 *            4096*n^4)*(2*n)!/(105*(n!)^2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* implemented as a(n) = (C(2n,n) * 16^n * P(n)) / 105 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Binomial coefficient C(2n, n) */
    seq_t bin = binomial(n << 1, n);

    /* Polynomial P(n) = 105 + 400*n + 3392*n^2 + 512*n^3 + 4096*n^4 */
    seq_t nn  = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t nn4 = nn3 * nn;

    seq_t poly = 105
               + 400 * nn
               + 3392 * nn2
               + 512 * nn3
               + 4096 * nn4;

    /* 16^n = 2^(4n) */
    seq_t pow16 = pow2((idx_t)(4 * n));

    /* Multiply the factors with exact division by 105 */
    seq_t denom  = 105;
    seq_t result = 1;
    seq_t factors[3] = {bin, pow16, poly};

    for (int i = 0; i < 3; ++i)
    {
        seq_t f = factors[i];

        /* cancel common part of f with the remaining denominator */
        seq_t g = gcd(f, denom);
        if (g > 1)
        {
            f /= g;
            denom /= g;
        }

        /* cancel common part of current result with the remaining denominator */
        g = gcd(result, denom);
        if (g > 1)
        {
            result /= g;
            denom /= g;
        }

        /* multiply the factor */
        result *= f;
    }

    /* If any factor of 105 remains, cancel it now */
    if (denom > 1)
    {
        result /= denom;
    }

    return result;
}
