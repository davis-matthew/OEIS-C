/*
 * A398672  Number of domino tilings (perfect matchings) of the 4 X 2n grid
 *          graph embedded on the Klein bottle.
 * https://oeis.org/A398672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3), a(0)=1, a(1)=16,
 *            a(2)=196, a(3)=2704 (order 3 recurrence)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Return cached value if available */
    if (n < cache_len) {
        return cache[n];
    }

    /* Determine new cache size, ensuring space for the base terms */
    idx_t new_len = n + 1;
    if (new_len < 4) new_len = 4;

    /* Reallocate cache */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed; return 0 as a fall‑back (should not happen in tests) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute and fill new terms */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)16;
        } else if (i == 2) {
            cache[i] = (seq_t)196;
        } else if (i == 3) {
            cache[i] = (seq_t)2704;
        } else {
            /* a(i) = 15*a(i-1) - 15*a(i-2) + a(i-3) */
            seq_t term = (seq_t)15 * cache[i-1];
            term -= (seq_t)15 * cache[i-2];
            term += cache[i-3];
            cache[i] = term;
        }
    }

    cache_len = new_len;
    return cache[n];
}
