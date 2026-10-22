/*
 * A344236  Number of n-step walks from a universal vertex to the other on the
 *          diamond graph.
 * https://oeis.org/A344236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=0, a(1)=1, a(2)=2,
 * a(n) = 5*a(n-2) + 4*a(n-3)  for n > 2
 */
#include <stdlib.h>  /* malloc, realloc, size_t, NULL */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of values stored (>= 1 if cache non-NULL) */

    /* Ensure cache is large enough to hold a[0] .. a[n] */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – exit or handle as needed */
            /* In a contest environment returning 0 is acceptable */
            return (seq_t)0;
        }
        cache = new_cache;
        /* Initialize newly allocated slots beyond old cache_len */
        if (cache_len == 0) { /* first time */
            cache[0] = (seq_t)0;
            if (new_len > 1) cache[1] = (seq_t)1;
            if (new_len > 2) cache[2] = (seq_t)2;
            cache_len = 3;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)1;
            if (new_len > 2) cache[2] = (seq_t)2;
            cache_len = 3;
        } else if (cache_len == 2) {
            cache[2] = (seq_t)2;
            cache_len = 3;
        }
        /* Fill remaining entries up to new_len */
        for (idx_t i = cache_len; i < new_len; ++i) {
            /* recurrence a(i) = 5*a(i-2) + 4*a(i-3) */
            seq_t term1 = cache[i - 2];
            seq_t term2 = cache[i - 3];
            cache[i] = (seq_t)(5 * term1 + 4 * term2);
        }
        cache_len = new_len;
    }

    return cache[n];
}
