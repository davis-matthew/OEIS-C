/*
 * A275865  Floor(r*a(n-1)) - floor(r*a(n-2)), where r = 3/2, a(0) = 1, a(1) =
 *          1.
 * https://oeis.org/A275865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor(3*a(n-1)/2) - floor(3*a(n-2)/2) Initial values:
 *            a(0) = 1, a(1) = 1
 * Range:     i8 n <= 27, i16 n <= 54, i32 n <= 109, i64 n <= 217,
 *            i128 n <= 437, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Floor division of an integer by 2. Works for negative numbers. */
static inline seq_t floor_div2(seq_t x)
{
    return (x >= 0) ? (x / 2) : ((x - 1) / 2);
}

/* floor(3*x/2) = x + floor(x/2) */
static inline seq_t floor_mul3_div2(seq_t x)
{
    return x + floor_div2(x);
}

/* Memoisation cache */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_capacity = 0; /* allocated size of cache */
static idx_t  max_cached = 1;     /* last index that has been computed */

/* Ensure that the cache can contain terms up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached)
        return;

    size_t new_capacity = (size_t)n + 1;
    if (new_capacity <= cache_capacity && new_capacity > 0)
        return;

    if (cache_capacity == 0) {
        cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    }
    if (!cache) abort();           /* allocation failed */

    cache_capacity = new_capacity;

    /* Base terms are always 1; set them on the first allocation. */
    if (cache_capacity >= 2) {
        cache[0] = 1;
        cache[1] = 1;
    }
}

/* Return the n‑th term of the sequence, any order of calls is supported. */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(0) = 1; a(1) = 1 */
        return 1;

    ensure_cache(n);

    if (max_cached >= n) /* term already computed */
        return cache[n];

    /* Compute missing terms starting from max_cached + 1 up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = floor_mul3_div2(prev1) - floor_mul3_div2(prev2);
    }

    max_cached = n;
    return cache[n];
}
