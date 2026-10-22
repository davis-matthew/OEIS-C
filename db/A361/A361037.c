/*
 * A361037  a(n) = 20*(3*n)!/((2*n)!*(n+2)!).
 * https://oeis.org/A361037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*(3*n)!/((2*n)!*(n+2)!)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=10; for n>=1, a(n) = a(n-1) * 3*(3*n-1)*(3*n-2) / ( 2*(n+2)*(2*n-1) ) */
#include <stdlib.h>

static seq_t *cache = NULL;    /* memoised values of a(n) */
static idx_t cache_cap = 0;    /* allocated capacity of cache */
static idx_t cache_len = 0;    /* number of values stored (cache_len-1 is max n cached) */

/* ensure cache has space for indices up to n inclusive */
static void ensure_cache(idx_t n) {
    if (cache_len > n) return;
    if (cache_cap < 16) cache_cap = 16;
    while (cache_cap <= n) cache_cap <<= 1;   /* double until large enough */
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

seq_t a(idx_t n) {
    if (cache_len == 0) {                 /* initialise with a(0)=10 */
        ensure_cache(0);
        cache[0] = 10;
        cache_len = 1;
    }
    if (cache_len <= n) {                 /* compute missing values up to n */
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t num = 3;
            num *= (seq_t)(3 * i - 1);
            num *= (seq_t)(3 * i - 2);
            seq_t den = 2;
            den *= (seq_t)(i + 2);
            den *= (seq_t)(2 * i - 1);
            cache[i] = cache[i - 1] * num / den;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
