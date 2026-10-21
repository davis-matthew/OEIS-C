/*
 * A249862  A special solution of X(n)^2 - 280*Y(n)^2 = 3^(2*n), n >= 0; here
 *          the X member.
 * https://oeis.org/A249862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - 9*a(n-2), a(0)=1, a(1)=17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   free_cache, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* memoisation cache: cache[0..len-1] are already computed */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity of cache   */
    static idx_t  len    = 0;   /* first unused index in cache   */

    size_t need = (size_t)n + 1;          /* how many elements are required */

    /* grow the cache if necessary */
    if (cap < need) {
        size_t newcap = cap ? cap : 2;    /* start with at least 2 slots */
        while (newcap < need) newcap <<= 1;  /* double until enough */

        seq_t *tmp = seq_alloc_new_array(newcap);   /* allocate new buffer   */
        if (cache) {                                 /* copy old data       */
            for (idx_t i = 0; i < cap; ++i)
                tmp[i] = cache[i];
            free_cache(cache);                       /* free old buffer     */
        }
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values up to index n */
    for (idx_t i = len; i < need; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 17;
        } else {
            cache[i] = 34 * cache[i-1] - 9 * cache[i-2];
        }
    }

    len = need;          /* everything up to n is now initialised */

    return cache[n];
}
