/*
 * A251654  4-step Fibonacci sequence starting with 0, 1, 1, 0.
 * https://oeis.org/A251654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) * a(0) = 0, a(1) = 1,
 *            a(2) = 1, a(3) = 0
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of elements currently stored  (up to cache_len-1) */
static size_t cache_cap = 0;     /* capacity of the allocated array */

/* Ensure that cache contains values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    /* Expand cache if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – abort (aoo). */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new terms if cache is incomplete. */
    idx_t i = (idx_t)cache_len;
    if (i <= n) {
        /* Initialize base values if cache empty. */
        if (i == 0) {
            cache[0] = (seq_t)0;
            ++i;
        }
        if (i == 1) {
            cache[1] = (seq_t)1;
            ++i;
        }
        if (i == 2) {
            cache[2] = (seq_t)1;
            ++i;
        }
        if (i == 3) {
            cache[3] = (seq_t)0;
            ++i;
        }

        /* Compute terms from i to n. */
        for (; i <= n; ++i) {
            cache[i] =
                cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
        }
        cache_len = (size_t)n + 1;
    }
}

/* Public function specified by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
