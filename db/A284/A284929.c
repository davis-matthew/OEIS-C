/*
 * A284929  Fixed point of the morphism 0 -> 01, 1 -> 1011.
 * https://oeis.org/A284929
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

/* Fixed point of morphism 0 → 01, 1 → 1011 */
/* The function builds the word iteratively and caches it across calls. */
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but return 0 safely */

    if (!cache) { /* initialise with the start word "0" */
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
    }

    while ((idx_t)cache_len < n) {        /* extend until we have at least n symbols */
        size_t new_len = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0) new_len += 2;
            else              new_len += 4;
        }

        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!tmp) abort();

        size_t pos = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0) {
                tmp[pos++] = 0;
                tmp[pos++] = 1;
            } else {
                tmp[pos++] = 1;
                tmp[pos++] = 0;
                tmp[pos++] = 1;
                tmp[pos++] = 1;
            }
        }

        free(cache);
        cache = tmp;
        cache_len = new_len;
    }

    return cache[n - 1];
}
