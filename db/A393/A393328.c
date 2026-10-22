/*
 * A393328  Number of total dominating sets in the n-necklace graph.
 * https://oeis.org/A393328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - a(n-2) + 21*a(n-3) + 6*a(n-4), a(0)=4,
 *            a(1)=11, a(2)=119, a(3)=1361
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL; /* cache of computed terms   */
static size_t  cap   = 0;   /* capacity of 'cache'      */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (n + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;

#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        if (old_cap > 0) {
            /* copy existing values */
            for (size_t i = 0; i < old_cap; ++i)
                new_cache[i] = cache[i];
        }
        delete[] cache;
        cache = new_cache;
#else
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure */
        cache = (seq_t *)tmp;
#endif
        cap = new_cap;

        /* Compute any missing terms, starting from old_cap */
        for (size_t i = old_cap; i < new_cap; ++i) {
            if (i == 0) {
                cache[i] = 4;
            } else if (i == 1) {
                cache[i] = 11;
            } else if (i == 2) {
                cache[i] = 119;
            } else if (i == 3) {
                cache[i] = 1361;
            } else {
                cache[i] = 11 * cache[i-1] - cache[i-2]
                           + 21 * cache[i-3] + 6 * cache[i-4];
            }
        }
    }

    return cache[(size_t)n];
}
