/*
 * A309307  Number of unitary divisors of n (excluding 1).
 * https://oeis.org/A309307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^omega(n) - 1 Where omega(n) is the number of distinct
 *            prime factors of n. Computation is performed by repeatedly
 *            extracting the smallest prime factor of n and counting distinct
 *            primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    uint64_t x = (uint64_t)n;
    int distinct = 0;
    while (x > 1) {
        uint64_t p = _smallest_prime_factor(x);
        distinct++;
        while (x % p == 0) x /= p;
    }

    /* compute 2^distinct - 1 using bit shift; seq_t supports << */
    seq_t val = ((seq_t)1 << distinct) - (seq_t)1;
    return val;
}
