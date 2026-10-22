/*
 * A398146  Expansion of generating function x / (1 - 5*x - x^2)^2.
 * https://oeis.org/A398146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2) - 10*a(n-3) - a(n-4) for n >= 5
 *            a(1) = 1, a(2) = 10, a(3) = 77, a(4) = 530 Any order of calls,
 *            cached for efficiency
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 to keep the function defined */
    if (n < 1u)
        return (seq_t)0;

    /* Static cache holds terms 1..cache_size (1‑based index). */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache;

#if defined(__cplusplus)
        /* Allocate a new array and default‑construct its elements. */
        new_cache = new seq_t[new_size];
#else
        /* Allocate memory for C (ptr must be cast). */
        new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_cache)
            abort();
#endif
        /* Copy previously computed terms. */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Compute missing terms from cache_size+1 up to n. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1u)
                val = (seq_t)1;
            else if (i == 2u)
                val = (seq_t)10;
            else if (i == 3u)
                val = (seq_t)77;
            else if (i == 4u)
                val = (seq_t)530;
            else
                val = 10*new_cache[i-2] - 23*new_cache[i-3]
                      - 10*new_cache[i-4] - new_cache[i-5];
            new_cache[i-1] = val;
        }

        /* Deallocate old cache. */
#if defined(__cplusplus)
        delete [] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n-1];
}
