/*
 * A376285  a(n) = 20^n * cos(n*A), where A is the angle opposite side BC in a
 *          triangle ABC having sidelengths |BC|=3, |CA|=4, |AB|=5; ABC is the
 *          smallest integer-sided right triangle.
 * https://oeis.org/A376285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 32*a(n-1) - 400*a(n-2), a(0) = 1, a(1) = 16. This
 *            implements the recurrence with memoisation
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Persistent cache of already computed terms */
    static seq_t *cache = NULL;       /* array of a[i] */
    static idx_t size   = 0;          /* allocated size of cache   */
    static idx_t last   = 0;          /* highest index that has
                                         been computed so far   */
    idx_t i;

    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 16;

    /* Initialise the cache on the first call that needs n>=2 */
    if (cache == NULL) {
        size = n + 1;  /* at least enough for n */
        /* Allocate and cast correctly in both C and C++ */
#ifdef __cplusplus
        cache = static_cast<seq_t*>(malloc(size * sizeof(seq_t)));
#else
        cache = (seq_t*)malloc(size * sizeof(seq_t));
#endif
        cache[0] = 1;
        cache[1] = 16;
        last = 1;
    }
    /* Grow the cache if the requested index is out of bounds */
    else if (size <= n) {
        idx_t newsize = n + 1;
#ifdef __cplusplus
        seq_t *newcache = static_cast<seq_t*>(malloc(newsize * sizeof(seq_t)));
#else
        seq_t *newcache = (seq_t*)malloc(newsize * sizeof(seq_t));
#endif
        /* Copy existing values to the new cache */
        for (i = 0; i < size; ++i)
            newcache[i] = cache[i];
        free(cache);
        cache = newcache;
        size  = newsize;
    }

    /* Compute any missing terms up to n */
    for (i = last + 1; i <= n; ++i)
        cache[i] = cache[i - 1] * 32 - cache[i - 2] * 400;

    last = n;
    return cache[n];
}
