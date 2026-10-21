/*
 * A291016  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 4*S + S^2.
 * https://oeis.org/A291016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) for n >= 3, a(0) = 4, a(1) = 19, a(2)
 *            = 90
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* dynamically growing cache of terms */
static size_t cache_size = 0;        /* number of terms currently stored */
static size_t cache_capacity = 0;    /* allocated capacity of the cache */

/* Ensure that the cache contains terms up to at least index n.
   Expands the array if necessary and computes missing terms. */
static void ensure_cache(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_capacity = 3;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)90;
        cache_size = 3;
    }

    /* grow capacity if needed */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = cache_capacity;
        if (new_cap == 0) new_cap = 3;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – abort (tests should not hit this) */
            abort();
        }
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)(6 * cache[i - 1] - 6 * cache[i - 2]);
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    /* return known initial values directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)90;

    /* use memoised recurrence for n >= 3 */
    ensure_cache(n);
    return cache[n];
}
