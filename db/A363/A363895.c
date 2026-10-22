/*
 * A363895  Floor of the average of the distinct prime factors of n.
 * https://oeis.org/A363895
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( (sum of distinct prime factors of n) / (count of
 *            distinct prime factors) )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t)0;
    }

    /* Allocate temporary arrays for the prime factorization. 64 is more than enough
       for 64‑bit integers (the product of the first 20 primes already exceeds 2^64). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i];
    }

    seq_t denom = (seq_t)cnt;
    return sum / denom;
}
