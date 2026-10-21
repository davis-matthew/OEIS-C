/*
 * A255447  A255446(2^n-1).
 * https://oeis.org/A255447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A255447:
 * Linear recurrence (from the generating function denominator):
 *   a(n) = 4*a(n-1) + 3*a(n-2) - 19*a(n-3) + 6*a(n-4) + 11*a(n-5)
 * Initial values (n = 0 … 4):
 *   a(0) = 1
 *   a(1) = 5
 *   a(2) = 21
 *   a(3) = 79
 *   a(4) = 291
 *
 * The function memoises computed terms so that subsequent calls (in any
 * order) are O(1).  All arithmetic uses the provided seq_t type.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed sequence values       */
static size_t  cache_len = 0; /* number of entries currently stored */
static size_t  cache_cap = 0; /* allocated capacity of the cache */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Convert to size_t safely; n is guaranteed to be within cache_cap. */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len)
        return; /* already computed */

    /* Expand capacity if necessary. */
    size_t new_cap = cache_cap;
    if (!new_cap) new_cap = 8;
    while (new_cap < needed)
        new_cap <<= 1;

    if (new_cap != cache_cap) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new values. */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)5;
        else if (i == 2)
            cache[i] = (seq_t)21;
        else if (i == 3)
            cache[i] = (seq_t)79;
        else if (i == 4)
            cache[i] = (seq_t)291;
        else {
            /* Apply the recurrence: a(n) = 4*a(n-1)+3*a(n-2)-19*a(n-3)+6*a(n-4)+11*a(n-5) */
            seq_t v = 4 * cache[i-1]           /* 4*a(n-1)  */
                    + 3 * cache[i-2]           /* 3*a(n-2)  */
                    - 19 * cache[i-3]          /* -19*a(n-3) */
                    + 6 * cache[i-4]           /* 6*a(n-4)  */
                    + 11 * cache[i-5];         /* 11*a(n-5) */
            cache[i] = v;
        }
    }

    cache_len = needed;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
