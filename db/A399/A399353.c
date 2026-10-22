/*
 * A399353  Number of independent vertex sets in the n X 3 projective plane
 *          graph.
 * https://oeis.org/A399353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-2) - 3*a(n-4) - 25*a(n-6) - 2*a(n-8) + a(n-10)
 *            with initial values a(0)=2, a(1)=3, a(2)=15, a(3)=42, a(4)=165,
 *            a(5)=497, a(6)=1894, a(7)=5759, a(8)=21857, a(9)=66560
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached sequence values; grows dynamically. */
static seq_t *cache = NULL;      /* pointer to stored values */
static idx_t max_cached = (idx_t)-1; /* highest index stored; -1 means empty */
static size_t cache_capacity = 0;    /* allocated capacity */

/* Initial 10 terms of the sequence. */
static const seq_t init_values[10] = {
    2ULL, 3ULL, 15ULL, 42ULL, 165ULL,
    497ULL, 1894ULL, 5759ULL, 21857ULL, 66560ULL
};

/* Ensure the cache has room for indices up to `n`.  Grow capacity
   geometrically to reduce repeated reallocations. */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: ensure capacity of at least 16. */
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();       /* allocation failure */
        /* Copy the initial values. */
        memcpy(cache, init_values, sizeof(init_values));
        /* Initialize the rest to zero. */
        for (size_t i = 10; i < cache_capacity; ++i)
            cache[i] = (seq_t)0;
        max_cached = 9;
        return;
    }

    if (n + 1 <= cache_capacity)
        return;  /* already enough space */

    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    if ((idx_t)new_cap < n + 1)
        new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure */
    cache = tmp;
    /* Zero-initialize new slots. */
    for (size_t i = cache_capacity; i < new_cap; ++i)
        cache[i] = (seq_t)0;
    cache_capacity = new_cap;
}

/* Compute a(n) using the recurrence and memoization. */
seq_t a(idx_t n)
{
    /* Make sure cache is initialized. */
    if (cache == NULL)
        ensure_capacity(n);

    if (n <= max_cached)
        return cache[n];

    /* Extend cache and compute missing terms iteratively. */
    ensure_capacity(n);
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i < 10) {
            cache[i] = init_values[i];
        } else {
            cache[i] = (12 * cache[i - 2]
                       - 3  * cache[i - 4]
                       - 25 * cache[i - 6]
                       - 2  * cache[i - 8]
                       + cache[i - 10]);
        }
    }
    max_cached = n;
    return cache[n];
}
