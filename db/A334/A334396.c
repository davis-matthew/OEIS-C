/*
 * A334396  Number of fault-free tilings of a 3 X n rectangle with squares and
 *          dominoes.
 * https://oeis.org/A334396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - a(n-3) - a(n-4) (n >= 5) a(1) = 0,
 *            a(2) = 0, a(3) = 2, a(4) = 2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Initial values ---------------------------------------------------- */
    static const seq_t init[5] = {0, 0, 0, 2, 2};

    /* Cache for memoisation -------------------------------------------- */
    static seq_t *cache = NULL;          /* array indexed from 1          */
    static idx_t  cache_size = 0;        /* allocated elements (including 0) */
    static idx_t  max_computed = 0;      /* largest n already computed     */

    /* Handle very small arguments ------------------------------------- */
    if (n <= 0) return 0;                /* value for the offset is 0     */
    if (n <= 4) return init[(size_t)n];

    /* Expand cache if necessary --------------------------------------- */
    if (cache_size <= n)
    {
#ifdef __cplusplus
        /* Allocate a new array and copy the old contents */
        seq_t *new_cache = new seq_t[(size_t)(n + 1)];
        for (idx_t i = 1; i < cache_size; ++i) new_cache[i] = cache[i];
        /* initialise new elements with zero (default constructor) */
        delete[] cache;
        cache = new_cache;
        cache_size = n + 1;
#else
        /* Reallocate using malloc/realloc (trivial types only) */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();                 /* allocation failure */
        cache = new_cache;
        cache_size = n + 1;
#endif
        /* initialise base cases if they were missing */
        if (max_computed < 4)
        {
            for (idx_t i = 1; i <= 4; ++i) cache[i] = init[(size_t)i];
            max_computed = 4;
        }
    }

    /* Compute all missing terms up to n --------------------------------- */
    for (idx_t i = max_computed + 1; i <= n; ++i)
        cache[i] = cache[i - 1] + 4 * cache[i - 2] -
                   cache[i - 3] - cache[i - 4];

    max_computed = n;
    return cache[n];
}
