/*
 * A396573  Primes p such that the gap to the next prime is strictly less than
 *          floor(log_2(p)).
 * https://oeis.org/A396573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_log2_uint64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A396573 – primes p such that the forward prime gap g <
   floor(log₂(p)).  The function returns the n‑th such prime (1‑based). */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/next_prime.h"          /* uint64_t next_prime(uint64_t); */
#include "../utils/floor_log2_uint64.h"   /* unsigned floor_log2_uint64(idx_t); */

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static cache of received terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next candidate to search for a new prime. */
    static uint64_t next_candidate = 2;   /* prime ≥ 2 */

    /* Grow cache if we need more terms. */
    while (cache_len < (size_t)n) {
        /* Ensure the cache array is large enough. */
        if (cache_cap == 0) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Find the next prime. */
        uint64_t p = next_prime(next_candidate);
        /* Prepare for next iteration: start after this prime. */
        next_candidate = p + 1;

        /* Find the succeeding prime to compute the gap. */
        uint64_t q = next_prime(p + 1);   /* always greater than p */

        uint64_t gap = q - p;
        unsigned log2p = floor_log2_uint64(p);

        /* Store p if it meets the criterion. */
        if (gap < log2p) {
            cache[cache_len++] = (seq_t)p;
        }
    }

    /* Return the required term (1‑based). */
    return cache[n - 1];
}
