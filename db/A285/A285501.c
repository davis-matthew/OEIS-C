/*
 * A285501  0-limiting word of the morphism 0->11, 1-> 0011.
 * https://oeis.org/A285501
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

/* Algorithm: computes the 0‑limiting word of the morphism
   0 → 11, 1 → 0011 by iteratively expanding the word until the
   requested index is reached and the number of iterations is even. */
#include <stdlib.h>

static seq_t *cache = NULL;   // current word
static idx_t cache_len = 0;   // length of current word
static idx_t iter = 0;        // number of iterations performed

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      // offset is 1

    /* initialise with the starting word "0" if not yet done */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)0;
        cache_len = 1;
        iter = 0;
    }

    /* expand until we have at least n symbols */
    while (cache_len < n) {
        idx_t zeros = 0, ones = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0)
                ++zeros;
            else
                ++ones;
        }
        idx_t new_len = zeros * (idx_t)2 + ones * (idx_t)4;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failed */

        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0) {
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
            } else {
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
            }
        }

        free(cache);
        cache = new_cache;
        cache_len = new_len;
        ++iter;
    }

    /* ensure we are at an even iteration (the 0‑limiting word) */
    if ((iter & 1UL) == 1) {
        idx_t zeros = 0, ones = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0)
                ++zeros;
            else
                ++ones;
        }
        idx_t new_len = zeros * (idx_t)2 + ones * (idx_t)4;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failed */

        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0) {
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
            } else {
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)1;
            }
        }

        free(cache);
        cache = new_cache;
        cache_len = new_len;
        ++iter;  // now even
    }

    return cache[n - 1];
}
