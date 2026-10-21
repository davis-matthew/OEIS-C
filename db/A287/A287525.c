/*
 * A287525  a(n) = a(n-1) + a(n-2) - a(n-3) +a(n-4) + a(n-5) for n >= 6, where
 *          a(0) = 2, a(1) = 4, a(2) = 7, a(3) = 12, a(4) = 19, a(5) = 31.
 * https://oeis.org/A287525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) + a(n-5), a(0) = 2,
 *            a(1) = 4, a(2) = 7, a(3) = 12, a(4) = 19, a(5) = 31
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values for indices 0..5 */
    static const seq_t init[6] = {2, 4, 7, 12, 19, 31};

    /* Cached values for all computed indices */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* number of cached elements (>= 6 once initialized) */

    /* Ensure we have space for at least n+1 entries */
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        if (new_cap < 6) new_cap = 6;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache = new_cache;
        /* Initialize new entries from previous cap */
        if (cap == 0) {
            for (size_t i = 0; i < (size_t)6; ++i) cache[i] = init[i];
        }
        if (n < cap) cap = new_cap; /* update cap early if no new entries needed */
        else {
            /* Compute missing entries from cap (previous size) up to new_cap-1 */
            for (size_t i = cap; i < new_cap; ++i) {
                if (i < 6) {
                    cache[i] = init[i];
                } else {
                    seq_t a1 = cache[i-1];
                    seq_t a2 = cache[i-2];
                    seq_t a3 = cache[i-3];
                    seq_t a4 = cache[i-4];
                    seq_t a5 = cache[i-5];
                    cache[i] = a1 + a2 - a3 + a4 + a5;
                }
            }
            cap = new_cap;
        }
    }
    /* Return the requested term */
    return cache[(size_t)n];
}
