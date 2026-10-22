/*
 * A359664  Prime Maze Room 11, opposite parity of A059459 starting from prime
 *          room 11.
 * https://oeis.org/A359664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Prime Maze Room 11 sequence (OEIS A359664).
 * a(1) = 11.  For n>1, a(n) is the first prime obtained by
 * toggling a single bit of a(n-1), starting at the least
 * significant bit and moving upwards, skipping the numbers
 * 2, 3 and any prime that has already appeared.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise the cache with the first term */
    if (cache_len == 0) {
        grow_seq(&cache, &cache_cap, (size_t)1);
        cache[0] = (seq_t)11;
        cache_len = 1;
    }

    /* Return early if the requested term is already cached */
    if ((idx_t)cache_len >= n) {
        return cache[n - 1];
    }

    /* Compute missing terms up to n */
    for (idx_t need = cache_len + 1; need <= n; ++need) {
        seq_t prev  = cache[cache_len - 1];
        seq_t mask  = (seq_t)1;
        seq_t next;

        /* search for the next term */
        while (true) {
            seq_t cand = prev ^ mask;

            if (cand != (seq_t)2 && cand != (seq_t)3 && is_prime_big(cand)) {
                /* ensure candidate has not appeared before */
                bool seen = false;
                for (size_t i = 0; i < cache_len; ++i) {
                    if (cache[i] == cand) { seen = true; break; }
                }
                if (!seen) {
                    next = cand;
                    break;
                }
            }

            /* advance to the next bit position */
            mask = mask * (seq_t)2;
        }

        /* extend cache if necessary and store the new term */
        if (cache_len == cache_cap) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
