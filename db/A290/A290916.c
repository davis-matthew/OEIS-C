/*
 * A290916  a(n) = (1/8)*A290915(n).
 * https://oeis.org/A290916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) + 4*a(n-3) - a(n-4), a(0)=0, a(1)=1,
 *            a(2)=4, a(3)=18
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;
    const seq_t init[4] = { 0, 1, 4, 18 };

    /* Handle the initial terms directly. */
    if (n < 4) return init[n];

    /* Allocate or grow the cache if necessary. */
    if (cached_size < 4) {
        cached_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cached_size);
        if (!cache) return 0; /* allocation failure; should not happen */
        for (idx_t i = 0; i < 4; ++i) cache[i] = init[i];
    }
    if (n >= cached_size) {
        idx_t new_size = n + 1;   /* enough to store up to n */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0; /* allocation failure; should not happen */
        cache = tmp;
        /* Compute missing terms up to n */
        for (idx_t k = cached_size; k <= n; ++k) {
            cache[k] = (seq_t)4 * cache[k - 1]
                     + (seq_t)2 * cache[k - 2]
                     + (seq_t)4 * cache[k - 3]
                     - cache[k - 4];
        }
        cached_size = new_size;
    }
    return cache[n];
}
