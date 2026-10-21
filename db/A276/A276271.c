/*
 * A276271  a(0) = a(1) = a(2) = a(3) = 1; for n > 3, a(n) =
 *          (a(n-1)^2+a(n-2)^2+a(n-3)^2+a(n-1)+a(n-2)+a(n-3))/a(n-4).
 * https://oeis.org/A276271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=a(3)=1; for n>3, a(n) = (a(n-1)^2 + a(n-2)^2 +
 *            a(n-3)^2 + a(n-1)+a(n-2)+a(n-3)) / a(n-4)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* trivial cases */
    if (n <= 3) return (seq_t)1;

    /* initialise cache with first four terms if not already done */
    if (cache == NULL) {
        cache = alloc_cache((idx_t)4);
        for (idx_t i = 0; i < 4; ++i) cache[i] = (seq_t)1;
        cache_len = 4;
    }

    /* ensure cache is large enough for requested index */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        seq_t *new_cache = alloc_cache((idx_t)(n + 1));
        for (idx_t i = 0; i < old_len; ++i) new_cache[i] = cache[i];
        free_cache(cache);
        cache = new_cache;
        cache_len = n + 1;    /* new cache length (valid indices 0..n) */

        /* compute missing terms */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t num = a1 * a1 + a2 * a2 + a3 * a3 + a1 + a2 + a3;
            cache[i] = num / cache[i - 4];
        }
    }

    return cache[n];
}
