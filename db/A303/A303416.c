/*
 * A303416  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2,
 *          3, 4 or 5 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 6*a(n-2) + 8*a(n-3) (n ≥ 4) initial values:
 *            a(1) = 4, a(2) = 32, a(3) = 252
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* a(n) stored at cache[n-1] */
    static size_t cache_len = 0;     /* number of terms cached */

    /* ensure cache large enough for n */
    size_t up_to = (size_t)n;        /* n is unsigned 64‑bit, safe to cast to size_t */
    if (up_to > cache_len) {
        size_t new_len = up_to;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 (undefined behaviour otherwise) */
            return (seq_t)0;
        }
        cache = new_cache;
        /* compute missing terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)4;
            } else if (i == 1) {
                cache[i] = (seq_t)32;
            } else if (i == 2) {
                cache[i] = (seq_t)252;
            } else {
                /* recurrence: a(n) = 7*a(n-1) + 6*a(n-2) + 8*a(n-3) */
                cache[i] =
                    (seq_t)7 * cache[i-1] +
                    (seq_t)6 * cache[i-2] +
                    (seq_t)8 * cache[i-3];
            }
        }
        cache_len = new_len;
    }
    return cache[n - 1];
}
