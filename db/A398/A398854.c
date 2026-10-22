/*
 * A398854  Lower (1/3, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), (a(0)=0,
 *            a(1)=1, a(2)=6, a(3)=14, a(4)=25)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial five terms of the sequence */
static const seq_t init_vals[5] = {0, 1, 6, 14, 25};

/* cache for memoisation, shared for all calls */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* number of elements currently cached */

seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    if (n < 5)
        return init_vals[n];

    /* allocate and initialise the cache if it does not exist yet */
    if (!cache) {
        cache    = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache)
            return 0;          /* allocation failed – caller may handle */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init_vals[i];
        cache_len = 5;
    }

    /* if the requested index is beyond the current cache, grow it */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;          /* need indices 0 … n  inclusive */

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* allocation failed */

        cache = tmp;
        /* initialise the new entries to zero (they will be overwritten) */
        for (idx_t i = old_len; i < new_len; ++i)
            cache[i] = 0;

        cache_len = new_len;

        /* compute the missing terms using the recurrence */
        for (idx_t i = old_len; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                     - cache[i - 2]
                     + cache[i - 3]
                     - 2 * cache[i - 4]
                     + cache[i - 5];
        }
    }

    return cache[n];
}
