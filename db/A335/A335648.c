/*
 * A335648  Partial sums of A006010.
 * https://oeis.org/A335648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + n)*(5 - 5*(-1)^n + 8*n + 12*n^2 + 8*n^3 + 2*n^4)/80
 *            Computation is performed via the 8‑term linear recurrence: a(n)
 *            = 4*a(n‑1) - 4*a(n‑2) - 4*a(n‑3) + 10*a(n‑4) - 4*a(n‑5) -
 *            4*a(n‑6) + 4*a(n‑7) - a(n‑8) for n > 7 The first 8 values are
 *            hard‑coded. A static cache of computed values all...
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 175, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

/* Return a(n) for the OEIS sequence A335648 */
seq_t a(idx_t n)
{
    /* Base values a(0) … a(7) */
    static const seq_t base[8] = {
        (seq_t)0,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)6,  /* a(2) */
        (seq_t)26, /* a(3) */
        (seq_t)78, /* a(4) */
        (seq_t)195,/* a(5) */
        (seq_t)420,/* a(6) */
        (seq_t)820 /* a(7) */
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries currently stored */

    /* Initialise cache with the first 8 elements */
    if (cache == NULL) {
        cache_size = 8;
        cache = alloc_seq_array(cache_size);
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = base[i];
    }

    /* Return base value if the request is among the first 8 */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to accommodate n */
    if (n >= cache_size) {
        /* Allocate a larger array */
        seq_t *new_cache = alloc_seq_array(n + 1);

        /* Copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Compute new entries using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val =
                new_cache[i-1] * (seq_t)4
                - new_cache[i-2] * (seq_t)4
                - new_cache[i-3] * (seq_t)4
                + new_cache[i-4] * (seq_t)10
                - new_cache[i-5] * (seq_t)4
                - new_cache[i-6] * (seq_t)4
                + new_cache[i-7] * (seq_t)4
                - new_cache[i-8];
            new_cache[i] = val;
        }

        /* Release the old cache and update pointers */
        free_cache(cache);
        cache = new_cache;
        cache_size = n + 1;
    }

    return cache[n];
}
