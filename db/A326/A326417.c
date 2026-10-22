/*
 * A326417  Dirichlet g.f.: zeta(s)^4 * (1 - 2^(-s)).
 * https://oeis.org/A326417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} * (e+1)(e+2)/2 if p = 2 * (e+1)(e+2)(e+3)/6
 *            if p is odd * (product over all prime powers dividing n) * a(1)
 *            = 1
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Multiplicative computation of A326417 */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;          /* local copy for factorisation */

    /* handle factor 2 separately */
    if (m % 2 == 0) {
        uint64_t e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        seq_t term = (seq_t)(e + 1) * (e + 2) / 2;
        result *= term;
    }

    /* factor odd primes up to sqrt(m) */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = (seq_t)(e + 1) * (e + 2) * (e + 3) / 6;
            result *= term;
        }
    }

    /* remaining prime factor > sqrt(original m) */
    if (m > 1) {
        /* m is prime, exponent e = 1 */
        seq_t term = (seq_t)(1 + 1) * (1 + 2) * (1 + 3) / 6;
        result *= term;
    }

    return result;
}
