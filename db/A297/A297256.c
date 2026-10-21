/*
 * A297256  Numbers whose base-5 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_variation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the nth positive integer whose base‑5
   up‑variation equals its down‑variation. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_variation.h"

/* Cache for previously computed terms */
static seq_t *cache = NULL;        /* array of values */
static size_t cache_len = 0;       /* number of cached terms */
static size_t cache_cap = 0;       /* capacity of the array */
static seq_t next_candidate = 1;   /* next integer to test */

/* Append a new value to the cache, growing the array if necessary. */
static void append_cache(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – terminate program (no safety path required). */
            abort();
        }
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* Public sequence function.  Returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for n = 0, but safe return */
    }

    /* Ensure the cache contains at least n terms. */
    while (cache_len < n) {
        while (!has_equal_variation(next_candidate)) {
            next_candidate++;
        }
        append_cache(next_candidate);
        next_candidate++;   /* move to next number for subsequent searches */
    }
    return cache[n - 1];
}
