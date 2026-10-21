/*
 * A283963  Fixed point of the morphism 0 -> 1, 1 -> 1010.
 * https://oeis.org/A283963
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

/* Construct the fixed point of the morphism 0 -> 1, 1 -> 1010
 * by iteratively expanding the word until the length is at least n.
 * The n-th element (1‑indexed) is returned. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cur_len = 0;

    if (n == 0) return 0;                /* safety: index starts at 1 */

    /* Initial cache contains the root symbol '1' */
    if (cache == NULL) {
        cache   = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;
        cur_len   = 1;
    }

    /* Grow the cache until it covers the requested index */
    while (cur_len < n) {
        /* Determine length after one more expansion */
        idx_t new_len = 0;
        for (idx_t i = 0; i < cur_len; ++i)
            new_len += (cache[i] == 0 ? 1 : 4);

        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            if (cache[i] == 0) {                 /* 0 → 1 */
                new_cache[pos++] = 1;
            } else {                             /* 1 → 1 0 1 0 */
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
                new_cache[pos++] = 1;
                new_cache[pos++] = 0;
            }
        }

        free(cache);
        cache = new_cache;
        cur_len = new_len;
    }

    return cache[n - 1];
}
