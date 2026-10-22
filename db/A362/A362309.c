/*
 * A362309  Expansion of e.g.f. exp(x - x^3/3).
 * https://oeis.org/A362309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - (n-1)*(n-2)*a(n-3); a(0)=a(1)=a(2)=1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 28, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoised terms */
static seq_t *cache      = NULL;          /* array of cached values      */
static size_t cache_cap = 0;              /* allocated length of cache   */
static size_t last_computed = (size_t)-1; /* last index that was computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache,
                                new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) exit(1);               /* fatal: memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) from the generating‑function recurrence. */
seq_t a(idx_t n)
{
    /* Initialise base case cache on the first call. */
    if (last_computed == (size_t)-1) {
        ensure_cache(2);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        last_computed = 2;
    }

    /* If the requested value is already cached, return it. */
    if (n <= last_computed)
        return cache[n];

    /* For n < 3 we fall through to the cached value. */
    if (n < 3)
        return cache[n];

    /* Compute missing values up to n. */
    ensure_cache(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-1) - (i-1)*(i-2)*a(i-3) */
        seq_t prod = (seq_t)(i - 1) * (seq_t)(i - 2) * cache[i - 3];
        cache[i] = cache[i - 1] - prod;
    }
    last_computed = n;
    return cache[n];
}
