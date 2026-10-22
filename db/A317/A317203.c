/*
 * A317203  Fixed under the morphism 1 -> 132, 2 -> 1, 3 -> 3, starting with
 *          31.
 * https://oeis.org/A317203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1200, u16 n <= 1200, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the morphism 1 → 132, 2 → 1, 3 → 3,
   starting with "31".  The function builds the word
   iteratively and caches the result for future calls. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached word: 1‑based indexed characters 1,2,3 */
    static seq_t *cache = NULL;
    static size_t size = 0;
    static int initialized = 0;

    if (!initialized) {
        /* initialise with the starting word "31" */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = 3;
        cache[1] = 1;
        size  = 2;
        initialized = 1;
    }

    /* grow the word until it covers position n */
    while (size < (size_t)n) {
        size_t cur_size = size;
        /* determine size of the next iteration */
        size_t new_size = 0;
        for (size_t i = 0; i < cur_size; ++i) {
            seq_t v = cache[i];
            if (v == 1) new_size += 3;   /* 1 → 1 3 2 */
            else       new_size += 1;   /* 2 → 1   or 3 → 3 */
        }

        /* build next iteration in a temporary array */
        seq_t *next = (seq_t *)malloc(new_size * sizeof(seq_t));
        size_t pos = 0;
        for (size_t i = 0; i < cur_size; ++i) {
            seq_t v = cache[i];
            if (v == 1) {
                next[pos++] = 1;
                next[pos++] = 3;
                next[pos++] = 2;
            } else if (v == 2) {
                next[pos++] = 1;
            } else {                   /* v == 3 */
                next[pos++] = 3;
            }
        }
        free(cache);
        cache = next;
        size  = new_size;
    }

    return cache[n - 1];
}
