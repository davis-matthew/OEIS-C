/*
 * A284368  Fixed point of the morphism 0->1, 1->1011.
 * https://oeis.org/A284368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth symbol in the fixed point of the morphism 0 → 1,
 *            1 → 1011. Starting word is “1”. The implementation builds the
 *            fixed point gradually, keeping the generated symbols in a static
 *            array; it expands the array until it contains at least n symbols
 *            and returns the n‑th symbol. Works for a...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;     /* array of 0/1 symbols */
static size_t cache_len = 0;    /* current number of stored symbols */

/* Return the nth symbol of the fixed point (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */

    /* Initialise the cache with the starting word "1" on the first call. */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        cache[0] = 1;
    }

    /* Expand the fixed point until it contains at least n symbols. */
    while (cache_len < (size_t)n) {
        size_t old_len = cache_len;
        size_t zeros = 0, ones = 0;

        /* Count zeros and ones in the current word. */
        for (size_t i = 0; i < old_len; ++i) {
            if (cache[i] == 0)
                ++zeros;
            else
                ++ones;
        }

        /* Length after one more expansion: 0→1, 1→1011. */
        size_t new_len = zeros + 4 * ones;

        /* Allocate memory for the expanded word. */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) abort();        /* allocation failure */

        /* Build the expanded word. */
        size_t j = 0;
        for (size_t i = 0; i < old_len; ++i) {
            if (cache[i] == 0) {
                new_cache[j++] = 1;       /* 0 → 1 */
            } else {
                new_cache[j++] = 1;       /* 1 → 1 0 1 1 */
                new_cache[j++] = 0;
                new_cache[j++] = 1;
                new_cache[j++] = 1;
            }
        }

        /* Replace the old word by the new word. */
        free(cache);
        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[n - 1];  /* 1‑based indexing */
}
