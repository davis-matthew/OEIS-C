/*
 * A308188  Fixed point (beginning with a) of the morphism a -> aab, b -> b,
 *          over the alphabet {a,b} = {1,2}.
 * https://oeis.org/A308188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   fixed point of morphism a->aab, b->b over alphabet {a,b} encoded
 *            as 1 for 'a' and 2 for 'b'
 * Range:     u8 n <= 8125, u16 n <= 8125, u32 n <= 8125, u64 n <= 8125,
 *            u128 n <= 8125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* cached prefix of the infinite word */
static idx_t  cache_len = 0;    /* length of the cached prefix */

seq_t a(idx_t n)
{
    if (n == 0)   /* never requested, but harmless */
        return (seq_t)0;

    /* Grow the cached word until it contains at least n symbols. */
    while (cache_len < n) {
        if (cache_len == 0) {                /* start with the first symbol 'a' = 1 */
            cache_len = 1;
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) abort();
            cache[0] = (seq_t)1;
            continue;
        }

        /* New length: each existing 'a' expands to 3 symbols, 'b' stays 1. */
        idx_t new_len = 2 * cache_len + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_cache) abort();

        idx_t r = 0, w = 0;
        while (r < cache_len) {
            seq_t v = cache[r++];
            if (v == (seq_t)1) {                     /* a -> aab -> 1 1 2 */
                new_cache[w++] = (seq_t)1;
                new_cache[w++] = (seq_t)1;
                new_cache[w++] = (seq_t)2;
            } else {                                 /* b -> b -> 2 */
                new_cache[w++] = (seq_t)2;
            }
        }

        free(cache);
        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[n - 1];
}
