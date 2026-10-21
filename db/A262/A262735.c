/*
 * A262735  Expansion of x*(1-x-x^2)/((1-x)*(1-x-2*x^2-x^3)).
 * https://oeis.org/A262735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) defined by the linear recurrence a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=4, a(5)=8, a(6)=17, a(n)=4*a(n-1)-4*a(n-2)+a(n-7)
 *            (n≥7)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   create_array, delete_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/create_array.h"
#include "../utils/delete_array.h"

/* Compute the n-th term of OEIS A262735. */
seq_t a(idx_t n) {
    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size of cache */
    static idx_t cached_max = 0;      /* largest index computed */
    static bool initialized = false;  /* whether we have set up cache */

    /* Constant initial values for n = 0..6. */
    static const seq_t base_vals[7] = {0, 1, 1, 2, 4, 8, 17};

    /* Initialise cache on first call. */
    if (!initialized) {
        cache_cap = 8;                     /* start with room for at least 8 entries */
        cache = create_array(cache_cap);
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = base_vals[i];
        for (size_t i = 7; i < cache_cap; ++i)
            cache[i] = 0;
        cached_max = 6;                    /* highest index we have computed */
        initialized = true;
    }

    /* Grow cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                 /* double until big enough */

        seq_t *new_cache = create_array(new_cap);
        for (size_t i = 0; i <= (size_t)cached_max; ++i)
            new_cache[i] = cache[i];
        for (size_t i = cached_max + 1; i < new_cap; ++i)
            new_cache[i] = 0;

        delete_array(cache, cache_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n. */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            if (i < 7) {
                cache[i] = base_vals[i];
            } else {
                /* a(n) = 4*a(n-1) - 4*a(n-2) + a(n-7) */
                cache[i] = 4 * cache[i - 1] - 4 * cache[i - 2] + cache[i - 7];
            }
        }
        cached_max = n;
    }

    return cache[n];
}
