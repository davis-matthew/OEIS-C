/*
 * A343597  Numbers divisible by a 7-smooth composite number.
 * https://oeis.org/A343597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   small_factor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A343597: numbers divisible by a 7‑smooth composite number. */
/* Equivalent to: a(n) is the n‑th positive integer that has at least two
 * prime factors among {2, 3, 5, 7}.  The elements are generated on‑demand
 * and cached so that repeated calls in any order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/small_factor_count.h"

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Safety: the offset for the sequence is 1; a(0) is unused but
     * returned as 0 for a defensive compilation. */
    if (n == 0)
        return (seq_t)0;

    /* Static state used for lazy evaluation and caching. */
    static uint64_t *cache = NULL;     /* array of 64‑bit terms */
    static size_t cache_len = 0;       /* number of cached terms */
    static size_t cache_cap = 0;       /* allocated capacity of cache */
    static uint64_t next_k = 1;        /* next integer to examine */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Scan forward to find the next qualifying integer. */
        while (1) {
            uint64_t k = next_k++;

            /* Accept k if it has at least two prime factors in {2,3,5,7}. */
            if (small_factor_count(k) >= 2) {
                /* Grow the cache if needed. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    uint64_t *new_ptr = (uint64_t*)realloc(cache, new_cap * sizeof(uint64_t));
                    if (!new_ptr) abort();    /* allocation failure */
                    cache = new_ptr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                break;      /* proceed to next needed term */
            }
        }
    }

    /* Return the requested term, casting to the build‑supplied seq_t. */
    return (seq_t)cache[n - 1];
}
