/*
 * A316955  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 4, 5 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 9*a(n-2) - 14*a(n-3) - 52*a(n-4) - 33*a(n-5)
 *            for n > 6, with initial values: a(1)=4, a(2)=32, a(3)=220,
 *            a(4)=1498, a(5)=10243, a(6)=70037
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t base_vals[6] = {
    (seq_t)4,
    (seq_t)32,
    (seq_t)220,
    (seq_t)1498,
    (seq_t)10243,
    (seq_t)70037
};

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for indices below the offset. */
        return 0;
    }

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of terms currently stored */

    if (cache_size < n) {
        /* Allocate additional space. */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – clear any existing cache. */
            if (cache) free(cache);
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = new_cache;

        /* Compute missing terms. */
        for (idx_t i = cache_size; i < n; ++i) {
            idx_t idx = i;  /* zero‑based index: a(1) is cache[0] */
            if (idx < 6) {
                cache[idx] = base_vals[idx];
            } else {
                /* Recurrence: a(n) = 6*a(n-1) + 9*a(n-2)
                   - 14*a(n-3) - 52*a(n-4) - 33*a(n-5)   */
                cache[idx] =
                    (seq_t)6 * cache[idx-1] +
                    (seq_t)9 * cache[idx-2] -
                    (seq_t)14 * cache[idx-3] -
                    (seq_t)52 * cache[idx-4] -
                    (seq_t)33 * cache[idx-5];
            }
        }
        cache_size = n;
    }

    return cache[n-1];
}
