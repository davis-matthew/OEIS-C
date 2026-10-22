/*
 * A312226  Coordination sequence Gal.3.18.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-4) + a(n-5) - a(n-9) for n > 10 initial values:
 *            a(0)=1, a(1)=4, a(2)=8, a(3)=14, a(4)=14, a(5)=22, a(6)=24,
 *            a(7)=34, a(8)=30, a(9)=38, a(10)=40
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial known terms up to n = 10 */
    static const seq_t init[11] = {
        1ULL, 4ULL, 8ULL, 14ULL, 14ULL,
        22ULL, 24ULL, 34ULL, 30ULL, 38ULL, 40ULL
    };
    const size_t init_len = 11;          /* number of elements in init */

    /* static cache for computed terms */
    static seq_t *cache = NULL;          /* array of computed a(k) values */
    static idx_t  cache_len = 0;         /* number of values stored */

    /* return directly if index within the initial block */
    if (n < init_len) {
        return init[n];
    }

    /* allocate cache if not yet done and copy initial values */
    if (cache == NULL) {
        cache_len = init_len;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure */
            return 0;
        }
        for (idx_t i = 0; i < init_len; ++i) {
            cache[i] = init[i];
        }
    }

    /* extend cache to include up to index n */
    if (n >= cache_len) {
        /* enlarge the array */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure */
            return 0;
        }
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = a(i-4) + a(i-5) - a(i-9) */
            cache[i] = cache[i - 4] + cache[i - 5] - cache[i - 9];
        }
        cache_len = new_len;
    }

    return cache[n];
}
