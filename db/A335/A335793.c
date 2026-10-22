/*
 * A335793  Number of hare pop-stack sortable Cayley permutations.
 * https://oeis.org/A335793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 4*a(n-3) for n > 3 a(0) = 1, a(1) =
 *            1, a(2) = 3, a(3) = 11 The implementation memoises the sequence
 *            in a static array. It works for both C and C++
 *            (arbitrary‑precision seq_t)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i < cache_size */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache          */
    static size_t cache_size = 0;  /* number of initialized elements   */

    /* Return the value if it is already cached. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Ensure the cache is large enough to hold indices 0 .. n. */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
#ifdef __cplusplus
        /* Allocate a new array (C++). */
        seq_t *new_cache = new seq_t[required];
        /* Copy existing values. */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* Destroy old array (C++). */
        delete[] cache;
        cache = new_cache;
        cache_cap = required;
#else
        /* Reallocate using realloc (C). */
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return zero (undefined for big). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = required;
#endif
    }

    /* Fill in any missing values up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else if (i == 3)
            cache[i] = (seq_t)11;
        else {
            /* Use the recurrence: a(i) = 5*a(i-1) - 6*a(i-2) + 4*a(i-3). */
            seq_t term =
                (seq_t)5 * cache[i-1]
                - (seq_t)6 * cache[i-2]
                + (seq_t)4 * cache[i-3];
            cache[i] = term;
        }
    }

    /* Update cache size and return the requested value. */
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
