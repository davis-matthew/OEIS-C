/*
 * A397975  Upper (1,2/3) midsequence triangular numbers (A000217) and squares
 *          (A000290); see Comments.
 * https://oeis.org/A397975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), a(0)=0,
 *            a(1)=2, a(2)=6, a(3)=12, a(4)=21
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache is allocated and initialized. */
    if (cache_size == 0) {
        cache_size = 5;                     /* Terms 0..4 */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* Out of memory: return 0 */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)21;
    }

    /* Extend the cache if the requested index is beyond current size. */
    if (n >= cache_size) {
        size_t old = (size_t)cache_size;
        size_t newsize = (size_t)(n + 1);

        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* Out of memory */
        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (size_t i = old; i < newsize; ++i) {
            /* Only compute when i >= 5; else use initial terms. */
            if (i < 5) continue;
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            cache[i] = (seq_t)2 * a1 - a2 + a3 - (seq_t)2 * a4 + a5;
        }
        cache_size = (idx_t)newsize;
    }

    /* Return the term. */
    return cache[n];
}
