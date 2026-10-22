/*
 * A363088  Positive numbers k for which sin(k) >= cos(k).
 * https://oeis.org/A363088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer k such that sin(k) >= cos(k)
 * (k is counted in the usual integer domain, sin/cos use radians).
 * The implementation scans k upward, appending those that satisfy the
 * inequality to a static cache.  Subsequent calls reuse the cached
 * values; the cache grows as needed.  The algorithm is O(n) time and
 * requires at most O(n) memory for the first n terms.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of found k's */
    static size_t cap   = 0;      /* current capacity of cache */
    static idx_t  cnt   = 0;      /* number of values currently stored */
    static idx_t  last  = 0;      /* last integer checked (k) */

    /* Ensure we have at least n terms cached. */
    while (cnt < n) {
        ++last;                               /* examine next integer k */
        double kd = (double)last;
        /* test sin(k) >= cos(k) */
        if (sin(kd) >= cos(kd)) {
            /* grow cache if needed */
            if (cnt == cap) {
                size_t newcap = cap ? cap * 2 : 4;
                seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
                if (!tmp) abort();
                if (cache) memcpy(tmp, cache, cnt * sizeof(seq_t));
                free(cache);
                cache = tmp;
                cap   = newcap;
            }
            cache[cnt++] = (seq_t)last;        /* store qualifying k */
        }
    }

    return cache[n - 1];
}
