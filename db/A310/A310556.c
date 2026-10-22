/*
 * A310556  Coordination sequence Gal.5.279.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n >= 14 initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=18, a(4)=22, a(5)=30, a(6)=28, a(7)=38,
 *            a(8)=46, a(9)=50, a(10)=58, a(11)=62, a(12)=58, a(13)=70
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#include <cstdlib>
#else
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n) {
    /* constants for the first 14 terms */
    static const seq_t base[14] = {
        1, 4, 10, 18, 22, 30, 28, 38, 46, 50, 58, 62, 58, 70
    };

    /* return directly if n is short */
    if (n < 14) {
        return base[n];
    }

    /* lazy static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the base values on first call */
    if (cache == NULL) {
        cache_size = 14;
#ifdef __cplusplus
        cache = new seq_t[cache_size];
        for (idx_t i = 0; i < cache_size; ++i) {
            cache[i] = base[i];
        }
#else
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        memcpy(cache, base, cache_size * sizeof(seq_t));
#endif
    }

    /* extend the cache up to the required index if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_size];
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }
        delete[] cache;
#else
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();
        new_cache = cache;  /* keep the pointer returned by realloc */
#endif
        cache = new_cache;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-6) - a(i-12) */
            cache[i] = cache[i - 6] * 2 - cache[i - 12];
        }
        cache_size = new_size;
    }

    return cache[n];
}
