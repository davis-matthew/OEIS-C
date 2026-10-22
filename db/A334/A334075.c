/*
 * A334075  a(n) is the denominator of the sum of reciprocals of primes not
 *          exceeding n and not dividing binomial(2*n, n).
 * https://oeis.org/A334075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the sum of reciprocals of primes p ≤ n
 *            that do NOT divide C(2n,n)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 86, u128 n <= 158,
 *            big any n
 * Helpers:   _exponent_in_binom, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for utilities used */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_exponent_in_binom.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* start with fraction 0/1 */
    seq_t num = 0, den = 1;

    /* Obtain all primes ≤ n */
    uint64_t *primes = NULL;
    size_t pcnt = 0;
    build_primes_upto(n, &primes, &pcnt);

    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        /* Skip primes that divide C(2n,n) */
        if (_exponent_in_binom(n, p) > 0)
            continue;

        /* Add 1/p to the current fraction num/den */
        seq_t new_den = den * (seq_t)p;
        seq_t new_num = num * (seq_t)p + den;
        seq_t g = gcd(new_num, new_den);
        den = new_den / g;
        num = new_num / g;
    }

    /* Clean up */
    free(primes);

    return den;
}
