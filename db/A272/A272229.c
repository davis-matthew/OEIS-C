/*
 * A272229  Numbers n such that n and 3n+1 have the same number of distinct
 *          prime divisors.
 * https://oeis.org/A272229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code: compute the A272229 terms by searching for integers n
 * satisfying ω(n) = ω(3n+1), where ω(m) is the number of distinct
 * prime factors of m.  The function memorizes the found terms
 * in a static cache so that subsequent calls are O(1).  The cache
 * grows as necessary and is limited only by available memory. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is 1‑based; 0 is undefined */
        return (seq_t)0;

    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored   */
    static size_t cache_cap  = 0;   /* capacity of the array    */
    static uint64_t next_candidate = 1;  /* last integer examined */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Search for the next integer m such that ω(m) == ω(3m+1). */
        for (;;) {
            ++next_candidate;                     /* candidate m          */

            /* Compute 3*m+1 safely in 128 bits to detect overflow. */
            unsigned __int128 t = (unsigned __int128)next_candidate * 3ULL + 1ULL;
            if (t > (unsigned __int128)UINT64_MAX)
                continue;                        /* skip if too large     */

            uint64_t m3 = (uint64_t)t;            /* 3*m+1                */

            int omega1 = count_distinct_primes(next_candidate);
            int omega2 = count_distinct_primes(m3);

            if (omega1 == omega2) {
                /* Ensure the cache has space. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {                     /* allocation failure   */
                        abort();
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
                break;                            /* proceed to next term  */
            }
        }
    }

    return cache[n - 1];
}
