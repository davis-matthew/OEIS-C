/*
 * A277369  a(0) = 5, a(1) = 8; for n>1, a(n) = 2*a(n-1) + a(n-2).
 * https://oeis.org/A277369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = 5, a(1) = 8
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a small cached array that grows as needed.
   It works both in C and in C++ (arbitrary‑precision seq_t is
   allocated with new[] / delete[] in C++ and with malloc / free in C). */

#include <stdlib.h>
#ifdef __cplusplus
# include <new>
#endif
#include <string.h>

/* Helpers to allocate / free seq_t arrays that work in C and C++. */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(sizeof(seq_t) * n);
#endif
}

static void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* pointer to the cached sequence */
    static size_t cache_cap = 0;         /* number of elements currently allocated */
    static idx_t last = (idx_t)-1;       /* largest index already computed (inclusive) */

    /* Initialise the cache with the two starting values if it hasn't been done */
    if (cache_cap < 2U) {
        size_t needed = 2U;
        cache = alloc_seq_array(needed);
        if (!cache) abort();             /* allocation failed */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)8;
        cache_cap = needed;
        last = 1;
    }

    /* Return the value immediately if it is already in the cache */
    if (n <= last)
        return cache[n];

    /* Grow the cache to hold elements up to n */
    if ((size_t)n + 1U > cache_cap) {
        size_t new_cap = (size_t)n + 1U;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) abort();             /* allocation failed */
        /* Copy the existing values to the new array */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute the missing terms iteratively using the recurrence */
    for (idx_t i = last + 1U; i <= n; ++i)
        cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 2];

    last = n;
    return cache[n];
}
