/*
 * A369737  a(n) = b(n, 1/2) where b(n, x) = (Pi/4)*(Y(0, x)*J(n, x) - J(0,
 *          x)*Y(n, x)) and Y, J are Bessel functions.
 * https://oeis.org/A369737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n)=4*(n-1)*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms; largest index computed is cache_n */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;   /* highest index currently stored */

    /* Ensure cache exists */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (cache == NULL) abort();          /* fatal error on allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        cache_n = 1;
    }

    /* If requested index is already cached, just return it */
    if (n <= cache_n) {
        return cache[n];
    }

    /* Extend the cache to at least index n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();                /* allocation failure */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        /* factor = 4 * (i-1) as seq_t */
        seq_t factor = (seq_t)(i - 1);
        factor = factor * (seq_t)4;
        cache[i] = factor * cache[i - 1] - cache[i - 2];
    }

    cache_n = n;
    return cache[n];
}
