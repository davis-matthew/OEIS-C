/*
 * A284505  Fixed point of the morphism 0->1, 1->1100.
 * https://oeis.org/A284505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   fixed point of the morphism 0->1, 1->1100. a(n) is 0 or 1
 *            (1‑based index)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* safety, not used in OEIS */

    /* cache holds the generated prefix as bytes 0/1, not as seq_t objects */
    static uint8_t *cache      = NULL;   /* cache of bits */
    static idx_t   cache_len   = 0;      /* actual length of cache */

    /* initialise with the first term (1) of the fixed point */
    if (cache_len == 0) {
        cache = (uint8_t *)malloc(sizeof(uint8_t));
        if (!cache) abort();            /* allocation error */
        cache[0] = 1;                  /* a(1) = 1 */
        cache_len = 1;
    }

    /* expand until we have at least n symbols */
    while (cache_len < n) {
        /* worst‑case length never exceeds 4 * cache_len */
        idx_t new_len_est = cache_len * 4;
        uint8_t *new_cache = (uint8_t *)malloc(new_len_est * sizeof(uint8_t));
        if (!new_cache) abort();

        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0) {
                /* 0 -> 1 */
                new_cache[pos++] = 1;
            } else {
                /* 1 -> 1 1 0 0 */
                new_cache[pos++] = 1;
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
                new_cache[pos++] = 0;
            }
        }

        free((void *)cache);
        cache = new_cache;
        cache_len = pos;
    }

    return (seq_t)cache[n - 1];
}
