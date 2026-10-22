/*
 * A346463  a(n) = 6 * GaussBinomial(2*n, 2, 2) / denominator(Bernoulli(2*n,
 *          1)).
 * https://oeis.org/A346463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n - 2)*(4^n - 1) / Cl(2*n,1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A346463 using the von Staudt–Clausen theorem
   for denominators of Bernoulli numbers. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 4^n */
    seq_t pow4 = seq_pow((seq_t)4, n);

    /* Denominator: product of primes p where (p-1) | 2n */
    seq_t denom = (seq_t)1;
    idx_t m = 2 * n;
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            uint64_t p = (uint64_t)(d + 1);
            if (is_prime(p))
                denom *= (seq_t)p;

            idx_t d2 = m / d;
            if (d2 != d)
            {
                p = (uint64_t)(d2 + 1);
                if (is_prime(p))
                    denom *= (seq_t)p;
            }
        }
    }

    /* Numerator factors: (4^n - 2) and (4^n - 1) */
    seq_t t1 = pow4 - (seq_t)2;
    seq_t t2 = pow4 - (seq_t)1;

    /* Reduce t1 and t2 by the denominator to avoid overflow */
    seq_t g = seq_gcd(t1, denom);
    t1 /= g;
    denom /= g;

    g = seq_gcd(t2, denom);
    t2 /= g;
    denom /= g;

    /* The remaining denominator should be 1, but in case
       it is not, perform the final division. */
    seq_t result = t1 * t2;
    if (denom > (seq_t)1)
        result /= denom;

    return result;
}
