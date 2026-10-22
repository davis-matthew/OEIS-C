/*
 * A398852  Lower (1/2, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5), a(0)
 *            = 0, a(1) = 1, a(2) = 6, a(3) = 15, a(4) = 27
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* ensure cache is large enough for n */
    if (n >= cache_size) {
        /* determine new size */
        size_t new_size = (size_t)n + 1;
        if (new_size < 5) new_size = 5;           /* at least store initial 5 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;                 /* out of memory: fallback */
        cache = new_cache;

        /* initialize first 5 terms if this is the first allocation */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)6;
            cache[3] = (seq_t)15;
            cache[4] = (seq_t)27;
        }
        /* compute missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i < 5) continue;          /* already initialized */
            cache[i] = 3 * cache[i-1]
                     - 4 * cache[i-2]
                     + 4 * cache[i-3]
                     - 3 * cache[i-4]
                     +   cache[i-5];
        }
        cache_size = new_size;
    }

    /* n < cache_size now */
    return cache[(size_t)n];
}
