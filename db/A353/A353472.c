/*
 * A353472  a(n) = 1 if n is a product of a prime and the square of a
 *          different prime (p^2 * q), otherwise 0.
 * https://oeis.org/A353472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = p² * q with p and q distinct prime, otherwise 0
 *            This implementation factors n and checks the pattern p²·q
 * Range:     u8 n <= 8178, u16 n <= 8178, u32 n <= 8178, u64 n <= 8178,
 *            u128 n <= 8178, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* Ensure uint64_t is available if needed */

seq_t a(idx_t n) {
    /* n < 12 cannot be of the form p²·q (the smallest is 2²·3=12) */
    if (n < 12) return 0;

    idx_t m = n;            /* copy of n to factor */
    int distinct = 0;       /* number of distinct prime factors */
    int total    = 0;       /* total number of prime factors counted with multiplicity */
    int two_exp  = 0;       /* exponent of the prime that might appear twice */

    /* Trial division up to sqrt(m). The test p <= m/p avoids overflow. */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            distinct++;
            while (m % p == 0) {
                m /= p;
                exp++;
                total++;
            }
            if (exp == 2)
                two_exp = exp;
            else if (exp > 2)          /* exponent > 2 cannot produce the required pattern */
                return 0;
        }
    }

    /* If something remains, it's a prime factor > sqrt(original n) */
    if (m > 1) {
        distinct++;
        total++;
    }

    /* The required form has exactly three prime factors (with multiplicity),
       exactly two distinct primes, and one of them should occur twice. */
    return (total == 3 && distinct == 2 && two_exp == 2) ? 1 : 0;
}
