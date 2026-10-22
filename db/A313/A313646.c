/*
 * A313646  Coordination sequence Gal.6.205.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n > 10 Initial values: a(0)=1, a(1)=5, a(2)=10,
 *            a(3)=14, a(4)=20, a(5)=26, a(6)=31, a(7)=36, a(8)=42, a(9)=48,
 *            a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* memoized sequence values */
static size_t cache_len = 0;       /* number of values currently cached */

/* Predefined initial values up to n=10 */
static const seq_t init_values[11] = {
    1, 5, 10, 14, 20, 26, 31, 36, 42, 48, 52
};

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = NULL;

        if (cache) {
            new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        }

        if (!new_cache) {
            /* Allocation failed – aborting as we cannot provide a correct value */
            exit(EXIT_FAILURE);
        }

        cache = new_cache;

        /* Initialize newly allocated slots */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i < 11) {
                cache[i] = init_values[i];
            } else {
                /* Recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) - a(n-10) */
                cache[i] = cache[i-1] + cache[i-3] - cache[i-4]
                           - cache[i-6] + cache[i-7]
                           + cache[i-9] - cache[i-10];
            }
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
