/*
 * A296816  a(n) = a(a(n-1)) + a(n-a(n-2)) with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A296816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1)) + a(n-a(n-2)), a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 465, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for previously computed values.
   cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;           /* indices 0 … n */
    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(*cache));
        if (!tmp) /* allocation failed – leave cache unchanged */
            return;
        /* Zero‑initialize the new slots */
        for (size_t i = cache_cap; i < needed; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = needed;
    }
}

/* Main sequence function.  Works for any seq_t and idx_t supplied by the
   build system, but assumes seq_t can hold all values that appear for the
   tested n. */
seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;

    ensure_cache(n);

    /* Initialise base cases if they haven't been set yet */
    if (cache[1] == (seq_t)0) {
        cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Compute values in increasing order */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0)        /* already computed */
            continue;

        /* Indices required by the recurrence */
        idx_t idx1 = (idx_t)cache[i - 1];        /* a(i-1) */
        idx_t idx2 = i - (idx_t)cache[i - 2];    /* i - a(i-2) */

        /* Both sub‑indices are guaranteed to be < i and thus previously
           computed. */
        cache[i] = cache[idx1] + cache[idx2];
    }

    return cache[n];
}
