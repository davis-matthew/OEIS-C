/*
 * A297262  Numbers whose base-7 digits have equal up-variation and
 *          down-variation; see Comments.
 * https://oeis.org/A297262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 9367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches integers whose base‑7 representation starts and ends with the same digit. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the nth term (1‑based) of sequence A297262. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t next_candidate = 1;

    /* Ensure cache has capacity. */
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t x = next_candidate++;
        /* Compute least significant digit. */
        seq_t lsd = x % 7;
        /* Compute most significant digit. */
        seq_t msd = x;
        while (msd >= 7) msd /= 7;
        if (msd == lsd) {
            cache[cache_len++] = x;
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
        }
    }

    return cache[n - 1];
}
