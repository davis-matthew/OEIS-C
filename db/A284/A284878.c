/*
 * A284878  Fixed point of the morphism 0 -> 01, 1 -> 0110.
 * https://oeis.org/A284878
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

/* Sequence A284878: fixed point of the morphism 0 → 01, 1 → 0110.
   The sequence is generated with a simple dynamic cache that expands
   using the morphism until the requested index is reached. */
#include <stdlib.h>

/* static cache that holds the produced word as 0/1 bytes */
static unsigned char *cache = NULL;
static idx_t cache_len = 0;          /* number of symbols stored */

/* Return the n‑th term (1‑based).  Undefined for n <= 0. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* safety guard, offset is 1 */
        return 0;

    /* Ensure the cache contains at least n symbols. */
    while (cache_len < n) {
        /* --- Initialization ------------------------------------------------ */
        if (cache_len == 0) {
            /* Start from the seed symbol 0. */
            cache = (unsigned char *)malloc(1);
            if (!cache) return 0;           /* allocation failure */
            cache[0] = 0;
            cache_len = 1;
            continue;
        }

        /* --- Expand the current word with the morphism ------------------- */
        /* Estimate new capacity: growth factor 4 (worst case) plus a small
           safety margin.  idx_t may be uint64_t, but the values we handle
           (n <= 10000) make this harmless. */
        size_t new_cap = cache_len * 4 + 4;
        unsigned char *new_word = (unsigned char *)malloc(new_cap);
        if (!new_word) return 0;          /* allocation failure */

        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0) {           /* 0 → 01 */
                new_word[pos++] = 0;
                new_word[pos++] = 1;
            } else {                       /* 1 → 0110 */
                new_word[pos++] = 0;
                new_word[pos++] = 1;
                new_word[pos++] = 1;
                new_word[pos++] = 0;
            }
        }

        /* Replace the old cache. */
        free(cache);
        cache = new_word;
        cache_len = pos;
    }

    /* Convert 1‑based index to 0‑based array index and return as seq_t. */
    return (seq_t)cache[n - 1];
}
