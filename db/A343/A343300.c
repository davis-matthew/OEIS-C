/*
 * A343300  a(n) is p1^1 + p2^2 + ... + pk^k where {p1,p2,...,pk} are the
 *          distinct prime factors in ascending order in the prime
 *          factorization of n.
 * https://oeis.org/A343300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..k} p_i^i where p_i are the distinct * prime
 *            factors of n in ascending order. a(1) = 0
 * Range:     u8 n <= 33, u16 n <= 245, u32 n <= 7709, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

static uint64_t spf_uint64(uint64_t n) {
    if (n <= 1) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return n;
}

seq_t a(idx_t n) {
    /* OEIS offset 1 → a(1)=0 */
    if (n <= 1) return 0;

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    size_t count = 0;

    while (m > 1) {
        uint64_t p = spf_uint64(m);
        primes[count++] = p;
        while (m % p == 0) {
            m /= p;
        }
    }

    seq_t sum = 0;
    for (size_t i = 0; i < count; ++i) {
        sum += big_pow((seq_t)primes[i], (seq_t)(i + 1));
    }
    return sum;
}
