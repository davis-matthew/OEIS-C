/*
 * A302748  Half thrice the previous number, rounded down, plus 1, starting
 *          with 6.
 * https://oeis.org/A302748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=6, a(n)=floor(3*a(n-1)/2)+1 for n>0
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_cap = 0;    /* current allocated capacity */
static size_t cache_len = 0;    /* number of terms stored */

/* Ensure that the cache has room for indices 0..need-1. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) {
        return;
    }

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < need) {
        new_cap <<= 1;               /* double until sufficient */
    }

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* The test harness guarantees enough memory; no error handling needed. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* idx_t is unsigned 64‑bit */
    ensure_cache_cap(idx + 1);

    /* Initialise first term if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)6;
        cache_len = 1;
    }

    /* Compute missing terms up to idx */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t prev = cache[i - 1];
            /* a(i) = prev + floor(prev/2) + 1 */
            cache[i] = prev + (prev >> 1) + (seq_t)1;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
