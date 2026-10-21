/*
 * A241606  A linear divisibility sequence of the fourth order related to
 *          A003779.
 * https://oeis.org/A241606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 25*a(n-2) + 11*a(n-3) - a(n-4); a(1)=1,
 *            a(2)=11, a(3)=95, a(4)=781
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* memoised terms */
static size_t  cache_cap = 0;      /* allocated size of cache     */
static size_t  cache_last = 0;     /* number of terms currently cached */

seq_t a(idx_t n)
{
    /* Direct return for the first four terms. */
    if (n <= 4) {
        static const seq_t base[4] = { 1, 11, 95, 781 };
        return base[n - 1];
    }

    /* Allocate or grow the cache to at least n elements. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();                     /* out of memory */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure the first four terms are present in the cache. */
    if (cache_last < 4) {
        static const seq_t base[4] = { 1, 11, 95, 781 };
        for (size_t i = cache_last; i < 4; ++i)
            cache[i] = base[i];
        cache_last = 4;
    }

    /* Compute missing terms up to the requested index. */
    for (size_t i = cache_last; i < (size_t)n; ++i) {
        /* recurrence: a(i+1) = 11*a(i) - 25*a(i-1) + 11*a(i-2) - a(i-3) */
        seq_t t = cache[i - 1] * 11;
        t -= cache[i - 2] * 25;
        t += cache[i - 3] * 11;
        t -= cache[i - 4];
        cache[i] = t;
    }
    cache_last = (size_t)n;

    return cache[n - 1];
}
