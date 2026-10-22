/*
 * A369273  Nonnegative numbers k satisfying sin(k) < sin(k+1) < sin(k+2).
 * https://oeis.org/A369273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A369273
 * Nonnegative integers k such that sin(k) < sin(k+1) < sin(k+2).
 *
 * Computation: iterate over k = 0,1,2,… and test sign of
 *   cos(k+0.5) and cos(k+1.5).
 *   When both >0 the inequality holds, so k is stored.
 *   The sequence is generated on demand and cached for future calls.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static cache of discovered terms */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;

    /* integer currently being tested */
    static idx_t k_test = 0;

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n) {
        double kd     = (double)k_test;
        double cos_05 = cos(kd + 0.5);   /* cos(k+0.5) */
        if (cos_05 <= 0.0) {
            ++k_test;
            continue;
        }
        double cos_15 = cos(kd + 1.5);   /* cos(k+1.5) */
        if (cos_15 <= 0.0) {
            ++k_test;
            continue;
        }

        /* Store k_test in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);           /* allocation failed */
            cache     = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)k_test;
        ++k_test;
    }

    return cache[n - 1];
}
