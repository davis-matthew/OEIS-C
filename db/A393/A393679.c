/*
 * A393679  Upper (1/2,1/3) midsequence of (n^2) and ((n+1)^2); see Comments.
 * https://oeis.org/A393679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393679: Upper (1/2,1/3) midsequence of (n^2) and ((n+1)^2)
   Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + a(n-8),
   with a(0..7) = (1, 2, 5, 10, 17, 25, 35, 46). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms currently cached */

    /* base values for indices 0..7 */
    static const seq_t base[8] = {
        (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)10,
        (seq_t)17, (seq_t)25, (seq_t)35, (seq_t)46
    };

    /* Initialise cache on the first call */
    if (!cache) {
        cache_size = 8;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(*cache));
        if (!cache) abort();                 /* allocation failed */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = base[i];
    }

    /* Extend cache if the requested index is beyond current size */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(*cache));
        if (!tmp) abort();                  /* allocation failed */
        cache = tmp;

        /* Compute missing terms up to index n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 8) continue;            /* base terms already set */
            seq_t val = 2 * cache[i - 1];
            val -= cache[i - 2];
            val += cache[i - 6];
            val -= 2 * cache[i - 7];
            val += cache[i - 8];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
