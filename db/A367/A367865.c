/*
 * A367865  a(n) = Sum_{d|n} d * phi(d) * mu(d)^2.
 * https://oeis.org/A367865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} d * φ(d) * μ(d)^2 Equivalent multiplicative
 *            form: a(p^e) = p^2 – p + 1, so a(n) = ∏_{p∣n} (p^2 – p + 1) for
 *            distinct primes p dividing n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* For n = 1 the product over empty set is 1. */
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* Trial division up to sqrt(m).  Use m/p to avoid overflow of p*p. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* distinct prime factor found */
            seq_t term = (seq_t)p * (seq_t)p - (seq_t)p + (seq_t)1;
            result *= term;

            /* Remove all powers of this prime from m */
            while (m % p == 0)
                m /= p;
        }
    }

    /* If a prime factor larger than sqrt(original n) remains it appears
       exactly once as a distinct prime factor. */
    if (m > 1) {
        seq_t term = (seq_t)m * (seq_t)m - (seq_t)m + (seq_t)1;
        result *= term;
    }

    return result;
}
