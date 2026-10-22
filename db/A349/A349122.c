/*
 * A349122  Inverse Möbius transform of A349128, where A349128(n) =
 *          phi(A064989(n)), A064989 is multiplicative with a(2^e) = 1 and
 *          a(p^e) = prevprime(p)^e for odd primes p, and phi is Euler totient
 *          function.
 * https://oeis.org/A349122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} f(p^e), where f(2^e) = e + 1, f(p^e) =
 *            prevprime(p)^e for odd primes p. This implements the
 *            multiplicative definition of A349122
 * Range:     u8 n <= 262, u16 n <= 9900, u32 n <= 9900, u64 n <= 9900,
 *            u128 n <= 9900, big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    idx_t m = n;
    /* Factor 2 separately. */
    int e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        result *= (seq_t)(e + 1);
    }

    /* Trial division by odd primes up to sqrt(m). */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t contrib = (seq_t)1;
            seq_t base = (seq_t)prev_prime((idx_t)p);
            for (int i = 0; i < e; ++i) {
                contrib *= base;
            }
            result *= contrib;
        }
    }

    /* If remaining m is a prime > 2. */
    if (m > 1) {
        /* m is odd prime with exponent 1. */
        seq_t contrib = (seq_t)prev_prime((idx_t)m);
        result *= contrib;
    }

    return result;
}
