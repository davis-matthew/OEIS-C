/*
 * A288206  a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 2*a(n-4), where a(0) = 2,
 *          a(1) = 4, a(2) = 8, a(3) = 18.
 * https://oeis.org/A288206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + 2*a(n-4), a(0) = 2, a(1) =
 *            4, a(2) = 8, a(3) = 18
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to allocate an array of seq_t in both C and C++ */
static seq_t *alloc_seq(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* Helper to resize an array of seq_t in both C and C++ */
static seq_t *resize_seq(seq_t *old, size_t old_cap, size_t new_cap)
{
#ifdef __cplusplus
    seq_t *newarr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i) newarr[i] = old[i];
    delete[] old;
    return newarr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len = 4;
        cache = alloc_seq(cache_cap);
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)18;
    }

    /* return if we already have the value */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* grow the cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < needed) new_cap <<= 1;
        cache = resize_seq(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t t1 = 3 * cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = 3 * cache[i - 3];
        seq_t t4 = 2 * cache[i - 4];
        cache[i] = t1 - t2 - t3 + t4;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
