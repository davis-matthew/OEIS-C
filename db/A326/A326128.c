/*
 * A326128  a(n) = n - A007913(n), where A007913 gives the squarefree part of
 *          n.
 * https://oeis.org/A326128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A007913(n), where * A007913(n) = product of the prime
 *            divisors of n whose exponent is odd
 * Range:     u8 n <= 274, u16 n <= 8336, u32 n <= 8336, u64 n <= 8336,
 *            u128 n <= 8336, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Square‑free part of n: product of primes with odd exponent */
    seq_t rad = (seq_t)1;
    idx_t m   = n;

    while (m > 1) {
        uint64_t p   = _smallest_prime_factor(m);
        int      odd = 0;                 /* e mod 2 for prime p */

        /* Count exponent of p in m, toggling parity */
        while (m % p == 0) {
            m /= p;
            odd ^= 1;
        }

        if (odd)                      /* odd exponent → multiply into rad */
            rad *= (seq_t)p;
    }

    /* a(n) = n - rad(n) */
    seq_t sn = (seq_t)n;
    return sn - rad;
}
