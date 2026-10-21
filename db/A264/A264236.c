/*
 * A264236  Number of vertices at level n of the hyperbolic Pascal pyramid.
 * https://oeis.org/A264236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 37*a(n-2) + 37*a(n-3) - 12*a(n-4) + a(n-5).
 *            Initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=13, a(4)=36
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of entries currently stored */

seq_t a(idx_t n) {
    size_t target = (size_t)n;

    /* Allocate and compute any missing entries up to target */
    if (target + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (target + 1) * sizeof(seq_t));
        if (!tmp) {                     /* allocation failed */
            if (cache) free(cache);
            cache = NULL;
            cache_size = 0;
            return (seq_t)0;
        }
        cache = tmp;

        for (size_t i = cache_size; i <= target; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)3;
            } else if (i == 2) {
                cache[i] = (seq_t)6;
            } else if (i == 3) {
                cache[i] = (seq_t)13;
            } else if (i == 4) {
                cache[i] = (seq_t)36;
            } else {
                seq_t term = (seq_t)12 * cache[i - 1];
                term -= (seq_t)37 * cache[i - 2];
                term += (seq_t)37 * cache[i - 3];
                term -= (seq_t)12 * cache[i - 4];
                term += cache[i - 5];
                cache[i] = term;
            }
        }

        cache_size = target + 1;
    }

    return cache[target];
}
