/*
 * A295733  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 0,
 *          a(1) = -1, a(2) = -1, a(3) = 1.
 * https://oeis.org/A295733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), a(0) = 0, a(1) =
 *            -1, a(2) = -1, a(3) = 1
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache     = NULL;   /* dynamically allocated array   */
    static idx_t cache_size = 0;     /* current capacity              */
    static idx_t computed   = 0;     /* greatest index computed so far */

    /* Expand the cache when necessary */
    if (n >= cache_size) {
        /* New capacity: double or at least enough for index n */
        size_t new_cap = cache_size ? cache_size * 2 : 4;
        while (new_cap <= (size_t)n)
            new_cap *= 2;

#ifdef __cplusplus
        /* In C++ use new[] / delete[] so that constructors are called */
        seq_t *tmp = new seq_t[new_cap];
        /* Copy existing values, if any */
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        /* In C use realloc (works with trivial types such as int, long, etc.) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp)
            abort(); /* allocation failure */
        cache      = tmp;
        cache_size = new_cap;

        /* Initialise the seed values only the first time */
        if (computed == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)-1;
            cache[2] = (seq_t)-1;
            cache[3] = (seq_t)1;
            computed = 3;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* i >= 4 because computed >= 3 before this loop starts */
        cache[i] = cache[i-1] + 3*cache[i-2] - 2*cache[i-3] - 2*cache[i-4];
    }
    if (computed < n)
        computed = n;

    return cache[n];
}
