/*
 * A314963  Coordination sequence Gal.6.199.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12, a(0)=1, a(1)=5,
 *            a(2)=9, a(3)=14, a(4)=20, a(5)=25, a(6)=31, a(7)=36, a(8)=42,
 *            a(9)=47, a(10)=51, a(11)=56, a(12)=61
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Initial values of the sequence for n = 0 .. 12. */
static const seq_t BASE[13] = {
    1, 5, 9, 14, 20, 25, 31, 36, 42, 47, 51, 56, 61
};

seq_t a(idx_t n)
{
    /* Memoization cache: cache[i] holds a(i) for 0 <= i <= last_filled. */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;    /* number of elements allocated      */
    static idx_t  last_filled = 0;    /* highest index that has a value    */

    /* Return base values directly if n < 13. */
    if (n < 13) {
        return BASE[n];
    }

    /* First request: allocate the cache and initialise the 13 base terms. */
    if (cache == NULL) {
        cache_len = 13;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure – unlikely */

        for (idx_t i = 0; i < 13; ++i) {
            cache[i] = BASE[i];
        }
        last_filled = 12;
    }

    /* Expand the cache if necessary. */
    if ((idx_t)(cache_len) <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – unlikely */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any values that have not yet been filled in. */
    while (last_filled < n) {
        ++last_filled;
        /* a(k) = a(k-1) + a(k-11) - a(k-12)  for k > 12 */
        cache[last_filled] = cache[last_filled - 1]
                             + cache[last_filled - 11]
                             - cache[last_filled - 12];
    }

    return cache[n];
}
