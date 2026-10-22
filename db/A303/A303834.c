/*
 * A303834  Number of total dominating sets in the n-gear graph.
 * https://oeis.org/A303834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 2*a(n-2) - 13*a(n-3) - 6*a(n-4) + 4*a(n-5)
   with initial values: a(1)=3, a(2)=21, a(3)=48, a(4)=161, a(5)=473. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for arbitrary‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    /* Direct answer for the first five terms */
    if (n <= 5) {
        static const seq_t init[6] = {0, 3, 21, 48, 161, 473};
        return init[n];
    }

    /* enlarge cache if necessary */
    if (n > cache_max) {
        idx_t new_max = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_max + 1) * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);      /* allocation failed */
        cache = new_cache;

        /* initialise the first five values if they are not yet set */
        if (cache_max < 5) {
            cache[0] = 0;
            cache[1] = 3;
            cache[2] = 21;
            cache[3] = 48;
            cache[4] = 161;
            cache[5] = 473;
        }

        /* compute new terms only */
        idx_t start = (cache_max >= 5 ? cache_max + 1 : 6);
        for (idx_t i = start; i <= new_max; ++i) {
            seq_t val = (seq_t)4 * cache[i-1];
            val += (seq_t)2 * cache[i-2];
            val += (seq_t)4 * cache[i-5];
            seq_t sub = (seq_t)13 * cache[i-3] + (seq_t)6 * cache[i-4];
            val -= sub;
            cache[i] = val;
        }
        cache_max = new_max;
    }
    return cache[n];
}
