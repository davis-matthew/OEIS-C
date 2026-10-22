/*
 * A386323  Primes without {0, 5} as digits.
 * https://oeis.org/A386323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 2937, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_or_five, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the nth prime whose decimal representation contains no digit 0 or 5. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/has_zero_or_five.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* For indices below the offset, behaviour is unspecified. */
    if (n == 0u)
        return 0;

    /* Static cache of valid primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 2u;

    /* Grow cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        /* Search for the next prime meeting the digit condition. */
        for ( ; ; ++next_candidate) {
            if (!is_prime(next_candidate))
                continue;
            if (has_zero_or_five(next_candidate))
                continue;

            /* Ensure cache capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0u) ? 32u : cache_cap * 2u;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }

            /* Store the found prime. */
            cache[cache_len++] = (seq_t)next_candidate;
            ++next_candidate;  /* move to the next number for future searches */
            break;
        }
    }

    return cache[(size_t)n - 1];
}
