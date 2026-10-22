/*
 * A353507  Product of multiplicities of the prime exponents (signature) of n;
 *          a(1) = 0.
 * https://oeis.org/A353507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of multiplicities of the exponents in the prime
 *            factorization of n; a(1) = 0. Uses a simple factorization
 *            routine from the utils library
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* We anticipate at most 20 distinct prime factors for a 64‑bit n. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the product of multiplicities of the exponents. */
    seq_t prod = (seq_t)1;
    bool seen[20] = {0};

    for (size_t i = 0; i < cnt; ++i) {
        if (seen[i])
            continue;
        unsigned mult = 1;
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exps[j] == exps[i]) {
                ++mult;
                seen[j] = true;
            }
        }
        prod *= (seq_t)mult;
    }
    return prod;
}
