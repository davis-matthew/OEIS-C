/*
 * A351266  Sum of the cubes of the squarefree divisors of n.
 * https://oeis.org/A351266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, changed
 * Formula:   a(n) = Sum_{d|n} d^3 * mu(d)^2 * Equivalently: a(n) = ∏_{p|n} (1
 *            + p^3) (product over distinct primes dividing n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;

    /* iterate over possible prime divisors up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* p is a distinct prime factor */
            seq_t pp = (seq_t)p * p * p;          /* p^3 */
            result *= (1 + pp);                   /* multiply by (1 + p^3) */
            /* remove all powers of p */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if any prime factor > sqrt(n) remains */
    if (temp > 1) {
        seq_t pp = (seq_t)temp * temp * temp;      /* remaining prime^3 */
        result *= (1 + pp);
    }

    return result;
}
