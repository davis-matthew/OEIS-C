/*
 * A346035  a(1) = 1; if a(n) is not divisible by 3, a(n+1) = 4*a(n) + 1,
 *          otherwise a(n+1) = a(n)/3.
 * https://oeis.org/A346035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 74, u64 n <= 186,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = (a(n) % 3 != 0) ? 4*a(n)+1 : a(n)/3 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* a(1) is cache[0] */
static size_t cache_len = 0;
static size_t computed = 0;   /* number of values stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 */
    size_t need = (size_t)n;       /* we need indices 0 … need-1 */

    /* first call: allocate cache for a(1) */
    if (cache == NULL) {
        cache_len = 1;
#ifdef __cplusplus
        cache = new seq_t[cache_len];
#else
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
#endif
        if (!cache) return 0;      /* allocation failure */
        cache[0] = (seq_t)1;       /* a(1) = 1 */
        computed = 1;
    }

    /* Grow the cache if needed */
    if (need > cache_len) {
        size_t new_len = cache_len;
        while (new_len < need) new_len <<= 1;   /* double until enough */

#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_len];
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete [] cache;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
        if (!tmp) {                 /* allocation failed */
#ifndef __cplusplus
            /* for C realloc failure cache still valid */
#endif
            return 0;
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms */
    for (size_t i = computed; i < need; ++i) {
        seq_t prev = cache[i-1];
        cache[i] = (prev % 3 != 0) ? prev * 4 + 1 : prev / 3;
    }
    computed = need;

    return cache[need-1];
}
