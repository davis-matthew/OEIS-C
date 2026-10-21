/*
 * A247619  Start with a single pentagon; at n-th generation add a pentagon at
 *          each expandable vertex; a(n) is the sum of all label values at
 *          n-th generation. (See comment for construction rules.)
 * https://oeis.org/A247619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-4*a(n-3)+2*a(n-4), a(0)=1, a(1)=6,
 *            a(2)=16, a(3)=36
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 245,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;        /* array of computed terms            */
    static size_t cache_cap = 0;       /* allocated capacity                  */
    static size_t cache_len = 0;       /* number of terms actually stored     */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)36;
        cache_len = 4;                 /* indices 0..3 stored                 */
    }

    /* enlarge cache if necessary so that index n is reachable   */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n + 1 > new_cap) {
            new_cap *= 2;
        }
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing terms up to n (inclusive)                  */
    for (idx_t i = cache_len; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t term;
        if (idx < 4) {
            /* already initialised above                          */
            term = cache[idx];
        } else {
            /* recurrence: a(i) = 2*a(i-1)+a(i-2)-4*a(i-3)+2*a(i-4) */
            term = 2 * cache[idx - 1] + cache[idx - 2]
                 - 4 * cache[idx - 3] + 2 * cache[idx - 4];
        }
        cache[idx] = term;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
