/*
 * A358027  Expansion of g.f.: (1 + x - 2*x^2 + 2*x^4)/((1-x)*(1-3*x^2)).
 * https://oeis.org/A358027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358027: a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) for n >= 5,
   with initial values a(0)=1, a(1)=2, a(2)=3, a(3)=6, a(4)=11. */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL; /* array of terms */
    static idx_t cache_size  = 0;    /* number of terms currently stored */

    /* initialise the cache with the first five terms on the first call */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)11;
    }

    /* if n is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* ensure the cache is large enough for indices up to n */
    idx_t needed = n + 1;
    if (needed > cache_size) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
        cache_size = needed;
    }

    /* compute all missing terms using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence: a[i] = a[i-1] + 3*a[i-2] - 3*a[i-3] */
        seq_t tmp = cache[i-1] + (seq_t)3 * cache[i-2];
        cache[i] = tmp - (seq_t)3 * cache[i-3];
    }

    return cache[n];
}
