/*
 * A336388  Number of prime divisors of sigma(n) that divide n; a(1) = 0.
 * https://oeis.org/A336388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p|sigma(n)} [p|n] (number of distinct prime factors
 *            of * sigma(n) that also divide n)
 * Range:     u8 n <= 999, u16 n <= 9167, u32 n <= 9167, u64 n <= 9167,
 *            u128 n <= 9167, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"   /* provides seq_t sigma_of(idx_t) */

seq_t a(idx_t n) {
    /* Compute σ(n) using the provided helper */
    seq_t sigma = sigma_of(n);

    /* σ(n) is small (≤ couple of hundred thousand for n ≤ 10000),
       so we can safely cast to uint64_t for factorisation. */
    uint64_t s = (uint64_t)sigma;

    /* Count distinct primes p|σ(n) that also divide n. */
    seq_t count = 0;

    /* Check factor 2 separately */
    if ((s & 1ULL) == 0) {
        if ((n & 1ULL) == 0) count++;          /* 2 divides n? */
        while ((s & 1ULL) == 0) s >>= 1;       /* divide out all 2's */
    }

    /* Check odd factors up to sqrt(s). */
    for (uint64_t i = 3; i * i <= s; i += 2) {
        if (s % i == 0) {
            if (n % i == 0) count++;           /* i divides n? */
            while (s % i == 0) s /= i;        /* divide out all i's */
        }
    }

    /* If any prime factor remains > 1, it is a single prime. */
    if (s > 1) {
        if (n % s == 0) count++;
    }

    return count;
}
