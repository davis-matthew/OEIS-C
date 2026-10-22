/*
 * A358042  Partial sums of A071619.
 * https://oeis.org/A358042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) a(0) = 0 a(1) = 1 a(2) = 4 a(3) = 10 a(4) = 21 a(5) = 38
 *            This implementation uses the linear recurrence with memoisation
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>  /* malloc, realloc */
#ifdef __cplusplus
#include <new>
#endif

/* static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t last_computed = 0;   /* largest index that has been computed */

/* grow the cache to accommodate indices 0..n */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache && cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap : needed;
    while (new_cap < needed)
        new_cap <<= 1;   /* geometric growth */

    if (!cache) {
#ifdef __cplusplus
        cache = new seq_t[new_cap];
#else
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    } else {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
#endif
    }
    cache_cap = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* initialise cache if it has not been created yet */
    if (!cache) {
        /* ensure capacity for the base terms (indices 0‑5) */
        grow_cache(5);
        /* ensure capacity for the requested index as well */
        grow_cache(n);

        /* set the first six values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)38;
        last_computed = 5;
    }

    /* if the term is already known, return it */
    if ((size_t)n <= last_computed)
        return cache[n];

    /* compute all necessary terms up to n */
    grow_cache(n);   /* ensure capacity after any earlier allocation */
    for (size_t idx = last_computed + 1; idx <= (size_t)n; ++idx) {
        seq_t pos = 3 * cache[idx-1] + 2 * cache[idx-3] + 3 * cache[idx-5];
        seq_t sub = 3 * cache[idx-2] + 3 * cache[idx-4] + cache[idx-6];
        cache[idx] = pos - sub;
    }

    last_computed = (size_t)n;
    return cache[n];
}
