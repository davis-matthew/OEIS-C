/*
 * A298367  Number of (n + 1, n + 2)-core partitions with each part repeated
 *          at most four times.
 * https://oeis.org/A298367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=5, a(3)=14, a(4)=42;
   for n >= 5: a(n)=a(n-1)+a(n-2)+2*a(n-3)+5*a(n-4)+14*a(n-5) */

#include <stdlib.h>

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of terms currently stored */
    static size_t cache_cap = 0; /* allocated capacity of the cache */

    /* Ensure we can address term n.  Keep a minimum capacity of 5 for
       the initial values. */
    size_t needed = (size_t)n + 1;
    if (needed < 5) {
        needed = 5;
    }

    if (cache_cap < needed) {
        /* Allocate a new larger buffer. */
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed: return 0 as a sentinel. */
            return (seq_t)0;
        }
        /* Copy any existing data into the new buffer. */
        if (cache != NULL) {
            for (size_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache = new_cache;
        cache_cap = needed;
        cache_len = 0;
    }

    /* Initialize the first five terms if they are not yet present. */
    if (cache_len < 5) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)42;
        cache_len = 5;
    }

    /* Compute any terms beyond what we have already. */
    for (size_t i = cache_len; i < needed; ++i) {
        cache[i] =
            cache[i - 1] +
            cache[i - 2] +
            (seq_t)2 * cache[i - 3] +
            (seq_t)5 * cache[i - 4] +
            (seq_t)14 * cache[i - 5];
    }

    cache_len = needed;
    return cache[(size_t)n];
}
