/*
 * A297980  Number of n X 2 0..1 arrays with every element equal to 0, 2, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 3*a(n-2) + 2*a(n-3) - 2*a(n-4) for n
 *            > 5 Initial values: a(1) = 1, a(2) = 5, a(3) = 6, a(4) = 17,
 *            a(5) = 24 (Sequence A297980)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 70, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑indexed cache of computed terms */
static idx_t cache_len = 0;     /* number of allocated slots (cache_len - 1 is max index stored) */
static const seq_t init_vals[6] = {0, 1, 5, 6, 17, 24};

seq_t a(idx_t n)
{
    /* Return early for the first five terms */
    if (n <= 5u) {
        return init_vals[n];
    }

    /* Allocate initial cache if necessary */
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) {
            abort();   /* allocation failure */
        }
        /* Set the initial five values */
        for (idx_t i = 1; i <= 5; ++i) {
            cache[i] = init_vals[i];
        }
        /* Compute terms up to n */
        for (idx_t i = 6; i <= n; ++i) {
            cache[i] = 3 * cache[i - 2] + 2 * cache[i - 3] - 2 * cache[i - 4];
        }
        return cache[n];
    }

    /* Grow the cache if n exceeds current capacity */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *new_cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (new_cache == NULL) {
            abort();   /* allocation failure */
        }
        /* Copy previously computed terms */
        for (idx_t i = 1; i < old_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;

        /* Compute new entries */
        idx_t start = old_len > 5 ? old_len : 6;  /* start from first missing index after the initial 5 */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 3 * cache[i - 2] + 2 * cache[i - 3] - 2 * cache[i - 4];
        }
    }

    return cache[n];
}
