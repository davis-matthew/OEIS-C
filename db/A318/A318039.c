/*
 * A318039  Number of nX2 0..1 arrays with every element unequal to 0, 2, 3 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) + a(n-4) - 2*a(n-5) - a(n-6)
 *            for n > 7. Initial values: a(1) = 1, a(2) = 2, a(3) = 2, a(4) =
 *            3, a(5) = 6, a(6) = 17, a(7) = 46 (These are the terms listed in
 *            OEIS A318039.)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build */
#include <stddef.h> /* for size_t */

/* Function prototype as required */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* largest index stored (0 if none) */
    static size_t cache_cap = 0;   /* current allocation capacity */

    /* Helper to grow the cache to at least 'required' elements. */
    if (cache_cap < (size_t)(cache_len + 1)) {
        size_t required = (size_t)(cache_len + 1);
        if (cache_cap < required) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            while (new_cap < required) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failed */
            cache    = tmp;
            cache_cap = new_cap;
        }
    }

    /* Initialise the first seven terms on the first call. */
    if (cache_len < 7) {
        /* Expand to at least 8 slots to allow index 0..7 */
        if (cache_cap < 8) {
            size_t new_cap = 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)6;
        cache[6] = (seq_t)17;
        cache[7] = (seq_t)46;
        cache_len = 7;
    }

    /* If the requested n is already computed, just return it. */
    if (n <= cache_len)
        return cache[n];

    /* Compute further values up to n using the recurrence. */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        /* Ensure enough space for the new element */
        if (cache_len + 1 >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Recurrence: a(i) = 3*a(i-1) - a(i-2) + a(i-3) + a(i-4) - 2*a(i-5) - a(i-6) */
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];
        seq_t v5 = cache[i-5];
        seq_t v6 = cache[i-6];

        seq_t val = (seq_t)3 * v1 - v2 + v3 + v4 - (seq_t)2 * v5 - v6;
        cache[(size_t)i] = val;
        cache_len = (size_t)i;
    }

    return cache[n];
}
