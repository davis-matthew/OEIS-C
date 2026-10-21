/*
 * A295316  a(n) = 1 if there are no even exponents in the prime factorization
 *          of n, 0 otherwise.
 * https://oeis.org/A295316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 9000, u16 n <= 9000, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if every prime exponent in the factorization of n is odd,
   0 otherwise (OEIS A295316). */

#include <stdint.h>   /* for uint64_t */

/* compute the sequence value for any n >= 1 */
seq_t a(idx_t n)
{
    /* 1 has no prime factors – the property trivially holds */
    if (n == 1) return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    /* handle factor 2 separately */
    if ((tmp & 1) == 0) {
        int exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            exp++;
        }
        if ((exp & 1) == 0) return (seq_t)0;   /* even exponent found */
    }

    /* trial division by odd primes up to sqrt(tmp) */
    for (uint64_t p = 3; p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if ((exp & 1) == 0) return (seq_t)0;  /* even exponent found */
        }
    }

    /* any remaining prime factor has exponent 1 (odd) */
    return (seq_t)1;
}
