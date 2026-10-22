/*
 * A316275  Lucas analog to A101361.
 * https://oeis.org/A316275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas(2*Fibonacci(n)); Recurrence: a(n+1) = a(n)*a(n-1) -
 *            a(n-2) Initial values: a(0)=2, a(1)=3, a(2)=3 Computes using the
 *            recurrence with memoisation
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values for all n already computed */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* number of allocated slots */
    static idx_t cache_max = (idx_t)-1; /* highest n computed, inclusive */

    /* initialise cache and base cases if needed */
    if (cache == NULL) {
        /* allocate enough for n up to the current request */
        cache_cap = (n > 2 ? n + 1 : 3);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();   /* out of memory */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)3;
        cache_max = 2;
    }
    /* grow cache if it cannot hold requested n */
    else if (cache_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
        /* new elements are initialised to zero; we will overwrite them */
    }

    /* compute forward until we reach n */
    while (cache_max < n) {
        /* cache_max currently holds the largest computed index */
        idx_t m = cache_max + 1;
        /* recurrence: a(m) = a(m-1)*a(m-2) - a(m-3) */
        cache[m] = cache[m - 1] * cache[m - 2] - cache[m - 3];
        cache_max = m;
    }

    return cache[n];
}
