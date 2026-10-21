/*
 * A253265  The number of tilings of 2 X n boards with squares of 2 colors and
 *          dominoes of 3 colors.
 * https://oeis.org/A253265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A253265
 *  Formula: a(0)=1, a(1)=7, a(2)=82,
 *  for n>=3: a(n)=10*a(n-1)+12*a(n-2)-27*a(n-3)
 *
 *  This function uses the recurrence derived from the generating function
 *  (1-3*x)/(1-10*x-12*x^2+27*x^3).  Results are cached so that any
 *  order of calls is fast and acceptable for n up to about 10,000.
 */

#include <stdlib.h>   /* malloc, realloc, size_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure we have computed all terms up to n */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1U;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache && new_size > 0) return 0;   /* allocation failed */
        cache = new_cache;

        /* Compute the sequence from the last computed index up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0U)
                cache[0] = 1;
            else if (i == 1U)
                cache[1] = 7;
            else if (i == 2U)
                cache[2] = 82;
            else
                cache[i] = 10 * cache[i - 1] + 12 * cache[i - 2] - 27 * cache[i - 3];
        }
        cache_size = new_size;
    }

    return cache[n];
}
