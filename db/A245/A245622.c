/*
 * A245622  Primes having more 1's digits than 3's.
 * https://oeis.org/A245622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Computes the nth prime whose decimal representation contains
 *            more 1's than 3's. The function is read on demand and caches all
 *            found values
 * Range:     u8 n <= 24, u16 n <= 2243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   more_ones_than_threes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/more_ones_than_threes.h"

/* Main sequence function.  Unordered calls are supported by caching. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0;     /* number of terms already cached */
    static size_t cap = 0;     /* allocated capacity of cache */

    static uint64_t *prime_list = NULL;
    static size_t prime_len = 0;
    static size_t prime_cap = 0;
    static uint64_t next_candidate = 2U; /* next number to test for primality */

    if (n == 0)
        return (seq_t)0; /* offset is 1, this value is irrelevant */

    /* Return cached value if already computed. */
    if (n <= len)
        return cache[n - 1];

    /* Initialise prime storage with the first prime 2. */
    if (prime_len == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2U;
        prime_len = 1;
    }

    /* Generate primes and filter until we have the requested nth value. */
    while (len < n) {
        uint64_t cand = next_candidate;

        /* Skip even numbers > 2. */
        if (cand > 2 && (cand & 1U) == 0)
            cand++;

        /* Primality test using previously found primes. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            if (prime_len == prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;

            /* If it satisfies the digit condition, cache it. */
            if (more_ones_than_threes(cand)) {
                if (len == cap) {
                    cap <<= 1;
                    if (cap == 0)
                        cap = 16;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = (seq_t)cand;
            }
        }

        /* Advance to the next candidate. */
        next_candidate = cand + 1;
    }

    return cache[n - 1];
}
