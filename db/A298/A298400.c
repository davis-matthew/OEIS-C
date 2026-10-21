/*
 * A298400  a(n) = a(n-1) + a(n-2) - n*a(floor(n/2)), where a(0) = 1, a(1) =
 *          1, a(2) = 1.
 * https://oeis.org/A298400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + a(n-2) - n*a(floor(n/2)), a(0)=1, a(1)=1, a(2)=1
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 45, i64 n <= 91, i128 n <= 183,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache   = NULL;        /* cache of computed values          */
static size_t cache_cap = 0;        /* current allocated capacity        */
static idx_t  cache_next = 0;       /* index of first not yet computed   */

seq_t a(idx_t n)
{
    /* Ensure the array can hold indices 0..n (inclusive). */
    ensure_cache(n, &cache, &cache_cap);

    if (n >= cache_next) {
        for (idx_t i = cache_next; i <= n; ++i) {
            if (i <= 2) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = cache[i-1] + cache[i-2]
                         - (seq_t)i * cache[i / 2];
            }
        }
        cache_next = n + 1;
    }
    return cache[n];
}
