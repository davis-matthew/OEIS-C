/*
 * A298339  a(n) = a(n-1) + a(n-2) + a([n/2]), where a(0) = 1, a(1) = 2, a(2)
 *          = 3.
 * https://oeis.org/A298339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) + a(floor(n/2)), with a(0)=1, a(1)=2,
 *            a(2)=3
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed values */
static size_t cache_cap = 0;      /* allocated capacity (in elements) */
static idx_t  computed = 0;       /* last index that has been computed */

/* Ensure the cache has space for at least n+1 elements and that values
   up to n are calculated. */
static void ensure_cache(idx_t n)
{
    /* Grow storage if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();       /* allocation failed */

        /* Copy existing values */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* Free old storage and update pointers/capacity */
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Supply base cases (overwritten if already present, harmless) */
    if (n >= 0) cache[0] = 1;
    if (n >= 1) cache[1] = 2;
    if (n >= 2) cache[2] = 3;

    /* Compute missing terms, starting after the already calculated range */
    idx_t start = computed + 1;
    if (start < 3) start = 3;          /* recurrence is valid for n >= 3 */

    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i >> 1];
    }

    if (n > computed) computed = n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
