/*
 * A253250  First differences of A049880.
 * https://oeis.org/A253250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049880(n+2) - A049880(n+1), where A049880(m) is the
 *            number of distinct sums of 2 different primes among the first m
 *            primes (computed by sieve and bitmap)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 118 terms through n = 977 (2026-11-07)
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/sieve_first_n_primes.h"

static seq_t A049880_a(idx_t n)
{
    if (n < 2) {
        return 0;
    }

    uint32_t *primes = NULL;
    size_t prime_count = 0;
    sieve_first_n_primes(n, &primes, &prime_count);

    uint32_t max_sum = primes[prime_count - 1] + primes[prime_count - 2];
    size_t bitmap_size = (size_t)max_sum + 1;

    uint8_t *seen = (uint8_t*)calloc(bitmap_size, sizeof(uint8_t));
    seq_t unique = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t pi = primes[i];
        for (size_t j = i + 1; j < prime_count; ++j) {
            uint32_t s = pi + primes[j];
            if (!seen[s]) {
                seen[s] = 1;
                ++unique;
            }
        }
    }

    free(primes);
    free(seen);

    return unique;
}

seq_t a(idx_t n)
{
    return A049880_a(n + 2) - A049880_a(n + 1);
}
