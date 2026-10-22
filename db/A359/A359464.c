/*
 * A359464  a(n) = 1 if the total number of 1-bits in the exponents of prime
 *          factorization n is even, otherwise 0.
 * https://oeis.org/A359464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 1 if the total number of 1-bits in the exponents of the
 *            prime factorization of n is even, otherwise 0
 * Range:     u8 n <= 9344, u16 n <= 9344, u32 n <= 9344, u64 n <= 9344,
 *            u128 n <= 9344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assuming idx_t is an unsigned 64‑bit integer and seq_t is an
   unsigned integer type (or arbitrary precision). */

#include <stdint.h>

/* parity of the bit‑count of a 64‑bit integer */
static inline unsigned int popcount_parity(uint64_t x) {
    unsigned int p = 0;
    while (x) {
        p ^= 1;
        x &= x - 1;            /* clear the lowest set bit */
    }
    return p;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* outside OEIS offset, not used */

    uint64_t m = (uint64_t)n;
    unsigned int parity = 0;

    /* factor 2 */
    uint64_t exp = 0;
    while ((m & 1ULL) == 0 && m > 0) {
        exp++;
        m >>= 1;
    }
    if (exp > 0) parity ^= popcount_parity(exp);

    /* factor odd primes up to sqrt(m) */
    for (uint64_t p = 3; p <= (m / p); p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            parity ^= popcount_parity(exp);
        }
    }

    /* any remaining prime factor */
    if (m > 1) {
        /* exponent is 1, popcount(1) == 1 */
        parity ^= 1;
    }

    /* parity == 0 => even total 1‑bits -> return 1 */
    return (parity == 0) ? (seq_t)1 : (seq_t)0;
}
