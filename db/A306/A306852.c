/*
 * A306852  a(n) = Sum_{k=0..floor(n/7)} binomial(n,7*k).
 * https://oeis.org/A306852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Sum_{k=0..floor(n/7)} binomial(n,7*k) */
/* via a 7‑term linear recurrence.  Initial values: */
/* a(0) = a(1) = … = a(6) = 1.  For n ≥ 7, */
/* a(n) = 7a(n-1) - 21a(n-2) + 35a(n-3) - 35a(n-4) + 21a(n-5) - 7a(n-6) + 2a(n-7). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
extern seq_t binomial(idx_t n, idx_t k);

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_used = 0;   /* next free index */

/* Utility to grow the cache when needed */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Convert to size_t for array indexing – safe for the supported n range */
    size_t idx = (size_t)n;

    /* Handle the initial 7 values directly. */
    if (idx < 7)
        return (seq_t)1;

    /* Initialise cache on first call */
    if (!cache) {
        /* Allocate enough room for 7 values; we will grow as needed. */
        cache_cap = 8;           /* start with 8 to avoid immediate realloc for n==7 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i)
            cache[i] = (seq_t)1;
        cache_used = 7;
    }

    /* Grow cache if the requested index is beyond current capacity */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        while (idx >= new_cap)
            new_cap <<= 1;           /* double until large enough */
        grow_cache(new_cap);
    }

    /* Compute missing values up to idx */
    for (size_t i = cache_used; i <= idx; ++i) {
        if (i < 7) {
            cache[i] = (seq_t)1;    /* safety for any missing lower indices */
        } else {
            seq_t t = 0;
            /* Positive contributions */
            t += cache[i - 1] * (seq_t)7;
            t += cache[i - 3] * (seq_t)35;
            t += cache[i - 5] * (seq_t)21;
            t += cache[i - 7] * (seq_t)2;
            /* Negative contributions */
            t -= cache[i - 2] * (seq_t)21;
            t -= cache[i - 4] * (seq_t)35;
            t -= cache[i - 6] * (seq_t)7;
            cache[i] = t;
        }
    }
    cache_used = idx + 1;   /* next free index */

    return cache[idx];
}
