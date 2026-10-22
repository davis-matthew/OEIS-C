/*
 * A366073  The number of composite "Fermi-Dirac primes" (A082522) dividing n.
 * https://oeis.org/A366073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e||n} floor(log_2(e))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the sum runs over the prime power factorisation of n. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;             /* n = 1 has no prime factors */

    seq_t result = 0;
    idx_t m = n;

    /* trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* compute floor(log2(exp)) for positive exp */
            seq_t term = 0;
            seq_t pow2 = 1;
            while (pow2 * 2 <= exp) {
                pow2 *= 2;
                ++term;
            }
            result += term;
        }
    }

    /* if m > 1, it is a prime factor with exponent 1 */
    /* floor(log2(1)) == 0, so no contribution */

    return result;
}
