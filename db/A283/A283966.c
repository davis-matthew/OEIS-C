/*
 * A283966  Fixed point of the morphism 0 -> 1, 1 -> 10101.
 * https://oeis.org/A283966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism 0 -> 1, 1 -> 10101.
   The function builds the word iteratively until it reaches
   at least the nth character and returns that character   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Guard against undefined behaviour for n < 1 */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Static cache holding the current prefix of the fixed point */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current length of the cache    */

    /* Initialise the cache with the seed word "1" on the first call */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Expand the cache until it covers the requested index */
    while (cache_len < n) {
        idx_t ones = 0, zeros = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i])
                ++ones;
            else
                ++zeros;
        }

        idx_t new_len = 5 * ones + zeros;   /* length after one morphism step */
        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_buf) return (seq_t)0;      /* allocation failure */

        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i]) {                 /* 1 -> 10101 */
                new_buf[pos++] = (seq_t)1;
                new_buf[pos++] = (seq_t)0;
                new_buf[pos++] = (seq_t)1;
                new_buf[pos++] = (seq_t)0;
                new_buf[pos++] = (seq_t)1;
            } else {                        /* 0 -> 1   */
                new_buf[pos++] = (seq_t)1;
            }
        }

        free(cache);
        cache = new_buf;
        cache_len = new_len;
    }

    /* Return the requested symbol (1‑based indexing) */
    return cache[n - (idx_t)1];
}
