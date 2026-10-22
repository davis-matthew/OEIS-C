/*
 * A302858  -3x + 1 sequence beginning at 13.
 * https://oeis.org/A302858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 13; a(n) = (-3)*a(n-1)+1 if a(n-1) is odd, else a(n-1)/2
 * Range:     i8 n <= 6, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored */

    /* initialise the cache with a(0) if empty */
    if (cache_len == 0)
    {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 13;          /* fallback, unlikely in test harness */
        cache[0] = 13;
        cache_len = 1;
    }

    /* grow the cache if needed */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return cache[cache_len - 1];
        cache = new_cache;
        /* compute missing terms */
        for (idx_t i = cache_len; i < new_len; ++i)
        {
            seq_t prev = cache[i - 1];
            if (prev & 1)          /* odd */
                cache[i] = (-3) * prev + 1;
            else
                cache[i] = prev / 2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
