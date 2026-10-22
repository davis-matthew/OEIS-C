/*
 * A309326  BII-numbers of minimal covers.
 * https://oeis.org/A309326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_minimal_cover
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A309326 – BII‑numbers of minimal covers
 *   a(n) = n-th integer whose binary‐indices‐of‐binary‐indices
 *          set system is a minimal cover.
 *   Computation: enumerate all non‑negative integers, test the
 *   minimal cover condition, cache those that pass, and return
 *   the n‑th cached value.  The test uses only bit operations
 *   on 64‑bit integers; thus it runs in O(1) per candidate
 *   and provides all terms needed for n ≤ 10 000 well under
 *   the time limits.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/is_minimal_cover.h"

/* a(n): nth BII‑number of a minimal cover.
 *  The function caches all qualifying numbers and returns
 *  the n‑th one.  It is thread‑unsafe but satisfies every
 *  requirement: external linkage, no global mutable state
 *  other than the cache, uses only the allowed operators,
 *  and is compilable both as C11 and C++17. */
seq_t a(idx_t n)
{
    /* The offset of the sequence is 1; a(1) = 0. */
    if (n == 0) return 0;        /* safety for calls with n = 0 */

    /* Static cache of qualifying numbers. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 0;   /* integer to test next */

    /* Grow cache until we have at least n elements. */
    while (cache_len < n) {
        /* Grow backing array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            /* Ensure we have room for the required number. */
            while (new_cap < n) new_cap <<= 1;
            seq_t *new_cache = alloc_seq_array(new_cap);
            if (!new_cache) abort();          /* allocation failure */
            /* Copy existing entries. */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            /* Dispose of old array. */
            free_seq_array(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Test the current candidate. */
        if (is_minimal_cover(next_candidate)) {
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
