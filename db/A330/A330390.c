/*
 * A330390  G.f.: (1 + 15*x) / (1 - 2*x - 17*x^2).
 * https://oeis.org/A330390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A330390
   Recurrence: a(0) = 1, a(1) = 17,
               a(n) = 2*a(n-1) + 17*a(n-2)  for n > 1
   (i.e. G.f.: (1 + 15*x)/(1 - 2*x - 17*x^2) ) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0]..a[len-1] are already computed */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   len   = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
#if defined(__cplusplus)
        cache = new seq_t[2];
#else
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
#endif
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        len = 2;
        cap = 2;
    }

    /* if n already computed, return it */
    if (n < len) return cache[(size_t)n];

    /* grow the cache if necessary */
    size_t target = (size_t)(n + 1);
    if (target > cap) {
        size_t new_cap = cap ? cap * 2 : 2;
        if (new_cap < target) new_cap = target;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
#endif
        cap = new_cap;
    }

    /* compute any missing terms */
    for (idx_t i = len; i <= n; ++i) {
        size_t idx = (size_t)i;
        cache[idx] = cache[idx-1] * (seq_t)2 + cache[idx-2] * (seq_t)17;
    }

    len = n + 1;
    return cache[(size_t)n];
}
