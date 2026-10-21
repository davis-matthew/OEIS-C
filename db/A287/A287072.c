/*
 * A287072  Start with 0 and repeatedly substitute 0->01, 1->21, 2->0.
 * https://oeis.org/A287072
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

/* Fixed point of morphism 0->01, 1->21, 2->0 */
/* The function builds the word iteratively until it contains the nth
 * symbol (1‑indexed) and then returns that symbol. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0 – return 0 */
        return (seq_t)0;

    /* static cache of the current word */
    static uint8_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache == NULL) {      /* initialise with the starting symbol 0 */
        cache     = (uint8_t *)malloc(1);
        cache[0]  = 0;
        cache_len = 1;
    }

    /* grow until we have at least n symbols */
    while (cache_len < (size_t)n) {
        /* compute length of the next word */
        size_t next_len = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            uint8_t c = cache[i];
            if (c == 2)
                next_len += 1;   /* 2 -> 0 */
            else
                next_len += 2;   /* 0 -> 01 , 1 -> 21 */
        }

        /* build the next word */
        uint8_t *next = (uint8_t *)malloc(next_len);
        size_t pos = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            uint8_t c = cache[i];
            if (c == 0) {
                next[pos++] = 0;
                next[pos++] = 1;
            } else if (c == 1) {
                next[pos++] = 2;
                next[pos++] = 1;
            } else {            /* c == 2 */
                next[pos++] = 0;
            }
        }

        free(cache);          /* discard previous word */
        cache     = next;
        cache_len = next_len;
    }

    return (seq_t)cache[n - 1];
}
