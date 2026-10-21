/*
 * A293880  Numbers having '20' as substring of their digits.
 * https://oeis.org/A293880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   contains20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293880: numbers having '20' as a decimal substring. */
/* a(n) = the nth positive integer whose decimal representation contains "20" */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains20.h"

/* static cache for memoization */
static seq_t *cache = NULL;          /* stored elements */
static size_t cache_len = 0;         /* number of elements stored */
static size_t cache_cap = 0;         /* allocated capacity */

/* ensure cache has at least 'needed' elements of space */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 32;
    while (needed > new_cap) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset starts at 1 */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* find next values until we have at least n terms */
    seq_t current = cache_len ? cache[cache_len - 1] : 0;
    while (cache_len < (size_t)n) {
        current++;
        if (contains20(current)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = current;
        }
    }
    return cache[(size_t)n - 1];
}
