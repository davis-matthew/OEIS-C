/*
 * A336652  Sum of positive divisors of odd part of n that are divisible by
 *          every (odd) prime dividing it: a(n) = A057723(A000265(n)).
 * https://oeis.org/A336652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over odd primes p^e||A000265(n) of (p + p^2 + … +
 *            p^e)
 * Range:     u8 n <= 188, u16 n <= 9131, u32 n <= 9131, u64 n <= 9131,
 *            u128 n <= 9131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below works for any idx_t > 0.  The odd part of n is
   obtained by shifting out all factors of 2.  For that odd part we
   factor it by trial division (up to sqrt), accumulate for each
   prime p a term equal to the sum of its powers up to e, and
   multiply those terms together.
   For powers of 2 (odd part 1) the function returns 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute odd part of n by stripping all factors of 2. */
    idx_t odd = n;
    while ((odd & (idx_t)1) == 0)
        odd >>= 1;

    /* a(1) = a(2^e) = 1 */
    if (odd == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = odd;

    /* trial division for odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            /* count exponent of p */
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }

            /* compute term = p + p^2 + … + p^exp */
            seq_t term = 0;
            seq_t power = 1;
            for (int i = 0; i < exp; ++i) {
                power *= (seq_t)p;
                term += power;
            }

            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1)
        result *= (seq_t)tmp;

    return result;
}
