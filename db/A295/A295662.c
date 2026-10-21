/*
 * A295662  Number of odd exponents larger than one in the canonical prime
 *          factorization of n.
 * https://oeis.org/A295662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime exponents e in the factorization of n
 *            such that e > 1 and e is odd
 * Range:     u8 n <= 9959, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n is at least 1; for n==1 return 0 directly */
    if (n <= 1) {
        return (seq_t)0;
    }

    uint64_t temp = (uint64_t)n;
    seq_t count = 0;

    /* Trial division up to sqrt(temp).  Use division to avoid overflow. */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* Count only odd exponents larger than 1. */
            if (exp > 1 && (exp & 1)) {
                ++count;
            }
        }
    }

    /* If temp > 1, it is a prime factor with exponent 1; not counted. */

    return count;
}
