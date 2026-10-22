/*
 * A330715  a(1), a(2), a(3) = 1; a(n) = (a(n-1) mod a(n-3)) + a(n-2) + 1.
 * https://oeis.org/A330715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = (a(n-1) mod a(n-3)) + a(n-2) + 1; a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 37, u16 n <= 517, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* safety for the offset, not part of the definition */
        return 0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_count = 0;   /* number of terms stored */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* compute the requested size as a size_t */
    size_t need = (size_t)n;

    /* grow the cache if necessary */
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *new_cache = alloc_cache(new_cap);
        if (cache) {
            memcpy(new_cache, cache, cache_count * sizeof(seq_t));
            free_cache(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_count; i < need; ++i) {
        if (i < 3) {                /* a(1) .. a(3) */
            cache[i] = 1;
        } else {
            seq_t a_nm1 = cache[i - 1]; /* a(n-1) */
            seq_t a_nm2 = cache[i - 2]; /* a(n-2) */
            seq_t a_nm3 = cache[i - 3]; /* a(n-3) */
            cache[i] = (a_nm1 % a_nm3) + a_nm2 + 1;
        }
    }

    cache_count = need;
    return cache[n - 1];
}
