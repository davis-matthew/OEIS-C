/*
 * A390836  Numerators of ratio n!/sigma(n), when the denominators are > 1 and
 *          where n! = factorial(n) (A000142) and sigma(n) = sum of divisors
 *          of n (A000203).
 * https://oeis.org/A390836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of reduced fraction n! / sigma(n) when sigma(n)
 *            does not divide n!
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Validate the index: n >= 1 is assumed by the caller */
    idx_t count = 0;
    seq_t fact = 1;     /* factorial(1) = 1 */

    for (idx_t i = 2; ; ++i)
    {
        /* Update factorial: fact = i! */
        fact = fact * (seq_t)i;

        /* Compute sigma(i) */
        seq_t sigma = sigma_of(i);

        /* g = gcd(i! , sigma) = gcd( fact % sigma , sigma ) */
        seq_t rem = fact % sigma;
        seq_t g   = gcd(rem, sigma);

        if (g != sigma)          /* denominator > 1, include this term */
        {
            seq_t numerator = fact / g;
            ++count;
            if (count == n)
                return numerator;
        }
        /* otherwise skip this n */
    }
}
