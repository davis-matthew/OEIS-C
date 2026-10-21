/*
 * A254602  Numbers of n-length words on alphabet {0..7} with no subwords ii,
 *          where i is from {0..2}.
 * https://oeis.org/A254602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 5*a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for all i <= max_cached */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of cached elements (i.e. size of the array) */

    /* allocate or grow cache to hold values up to n */
    if (cached_len <= n) {
        /* new size is n+1 */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed; abort (no error handling required) */
            exit(1);
        }
        cache = new_cache;
        /* initialize new entries */
        for (idx_t i = cached_len; i < new_len; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)8;
            else
                cache[i] = (seq_t)7 * cache[i - 1] + (seq_t)5 * cache[i - 2];
        }
        cached_len = new_len;
    }
    return cache[n];
}
