/*
 * A269284  Number of n X 3 0..3 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling three
 *          no more than once.
 * https://oeis.org/A269284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * 3^n * 7^(n-2) * (24*n+11); Recurrence: a(1)=60,
 *            a(2)=2124, a(n)=42*a(n-1) – 441*a(n-2) for n≥3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static memoisation tables */
    static seq_t *cache   = NULL;   /* array of terms a(1) … a(max)  */
    static size_t cache_sz = 0;     /* allocated size of cache       */
    static size_t computed = 0;     /* largest index already computed */

    /* ensure the cache is large enough for requested n (at least 2 entries) */
    size_t needed = (size_t)n;
    if (needed < 2) needed = 2;
    if (cache_sz < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
        cache = tmp;
        cache_sz = needed;
    }

    /* initialise base values if not already present */
    if (computed < 1) {
        cache[0] = (seq_t)60;   /* a(1) */
        computed = 1;
    }
    if (computed < 2) {
        cache[1] = (seq_t)2124; /* a(2) */
        computed = 2;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 2];      /* a(i-1) */
        seq_t a2 = cache[i - 3];      /* a(i-2) */
        cache[i - 1] = (seq_t)42 * a1 - (seq_t)441 * a2;
    }
    computed = (size_t)n;   /* update the highest computed index */

    return cache[n - 1];
}
