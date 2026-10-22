/*
 * A315543  Coordination sequence Gal.4.137.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - a(n-8); Initial values: a(0)=1, a(1)=6,
 *            a(2)=11, a(3)=17, a(4)=23, a(5)=29, a(6)=35, a(7)=40, a(8)=46
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[9] = {
    1, 6, 11, 17, 23, 29, 35, 40, 46
};

static seq_t *cache     = NULL;      /* array holding computed terms */
static size_t known_up_to = 8;       /* highest index that has been computed */
static size_t cache_cap  = 0;        /* capacity of 'cache' array */

seq_t a(idx_t n)
{
    /* Return from the initial table for the first nine terms */
    if (n < 9)
        return init_vals[n];

    /* Allocate cache on first use (size at least 9) */
    if (cache == NULL) {
        cache_cap = 16;                          /* start with some space */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failure */

        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        known_up_to = 8;
    }

    /* Ensure there is enough room for index 'n' */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to 'n' if necessary */
    if (n > known_up_to) {
        for (size_t i = known_up_to + 1; i <= (size_t)n; ++i) {
            seq_t sum_plus  = 2*cache[i-1] + 2*cache[i-3] + 2*cache[i-5] + 2*cache[i-7];
            seq_t sum_minus = 2*cache[i-2] + 2*cache[i-4] + 2*cache[i-6] + cache[i-8];
            cache[i] = sum_plus - sum_minus;
        }
        known_up_to = (size_t)n;
    }

    return cache[n];
}
