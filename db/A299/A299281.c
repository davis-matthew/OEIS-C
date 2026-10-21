/*
 * A299281  Coordination sequence for "reo-e" 3D uniform tiling.
 * https://oeis.org/A299281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A299281 – Coordination sequence for "reo-e" 3D uniform tiling
 *  Recurrence (for n >= 11):
 *      a(n) = 3*a(n-1) - 5*a(n-2) + 7*a(n-3) - 7*a(n-4) + 5*a(n-5)
 *             - 3*a(n-6) + a(n-7)
 *  Initial values (from the OEIS entry):
 *      a(0)=1, a(1)=6, a(2)=19, a(3)=41, a(4)=72, a(5)=114, a(6)=166,
 *      a(7)=224, a(8)=288, a(9)=364, a(10)=454
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[11] = {
    (seq_t)1,  (seq_t)6,  (seq_t)19, (seq_t)41, (seq_t)72,
    (seq_t)114, (seq_t)166, (seq_t)224, (seq_t)288, (seq_t)364,
    (seq_t)454
};

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_count = 0;   /* number of elements stored (len) */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* Handle the first 11 values directly */
    if (n < 11) {
        return init_vals[(size_t)n];
    }

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure – return 0 as sentinel */
            return (seq_t)0;
        }
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init_vals[i];
        }
        cache_count = 11;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) {
            new_cap = (size_t)(n + 1);
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – return 0 as sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n using the recurrence */
    for (size_t i = cache_count; i <= (size_t)n; ++i) {
        seq_t pos = (cache[i - 1] * (seq_t)3) + (cache[i - 3] * (seq_t)7)
                  + (cache[i - 5] * (seq_t)5) + cache[i - 7];
        seq_t neg = (cache[i - 2] * (seq_t)5) + (cache[i - 4] * (seq_t)7)
                  + (cache[i - 6] * (seq_t)3);
        /* pos must be >= neg for all valid n; use unsigned subtraction */
        cache[i] = pos - neg;
    }

    cache_count = (size_t)n + 1;
    return cache[(size_t)n];
}
