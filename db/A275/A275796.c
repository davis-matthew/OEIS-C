/*
 * A275796  One half of the y members of the positive proper solutions (x =
 *          x2(n), y = y2(n)) of the second class for the Pell equation x^2 -
 *          2*y^2 = +7^2.
 * https://oeis.org/A275796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) (n ≥ 1) a(0) = 3, a(1) = 20 Recurrence
 *            derived from the OEIS entry for A275796
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* The sequence cache and its metadata (static internal linkage). */
static seq_t *cache = NULL;    /* array of computed a(n) values */
static size_t cache_len = 0;   /* number of values currently stored */
static size_t cache_cap = 0;   /* allocated capacity (in seq_t elements) */

/* Grow the cache to at least the requested capacity. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    /* allocate a larger buffer: use at least double the previous size
       to keep amortized O(1) cost, but also ensure we have enough space. */
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    if (new_cap < needed) new_cap = needed;

    if (cache == NULL) {
        /* first allocation */
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        /* resize */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
    }
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS A275796.  idx_t is unsigned (uint64_t). */
seq_t a(idx_t n)
{
    /* Ensure that the cache has the first two values when it is empty. */
    if (cache_len == 0) {
        grow_cache(2);
        cache[0] = (seq_t)3;   /* a(0) */
        cache[1] = (seq_t)20;  /* a(1) */
        cache_len = 2;
    }

    /* Extend the cache up to and including index n if necessary. */
    if ((size_t)n >= cache_len) {
        /* Reserve space for all needed elements. */
        grow_cache((size_t)n + 1);

        /* Compute missing terms using the recurrence. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* a(i) = 6*a(i-1) - a(i-2) */
            seq_t val = (seq_t)6 * cache[i - 1] - cache[i - 2];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    /* Index is valid now. */
    return cache[(size_t)n];
}
