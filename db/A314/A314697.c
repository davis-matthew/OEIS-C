/*
 * A314697  Coordination sequence Gal.5.125.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A314697: Coordination sequence Gal.5.125.4
 *
 * Recurrence (n ≥ 18):
 *   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) - a(n-6)
 *          + 2*a(n-7) - a(n-8) - a(n-9) + 2*a(n-10)
 *          - a(n-11) + a(n-13) - a(n-14)
 * Initial values:
 *   a(0)=1, a(1)=5, a(2)=9, a(3)=13, a(4)=17, a(5)=24, a(6)=27,
 *   a(7)=34, a(8)=35, a(9)=44, a(10)=45, a(11)=52, a(12)=53,
 *   a(13)=62, a(14)=65, a(15)=70, a(16)=71, a(17)=80
 */

#include <stdlib.h>

static const seq_t base_vals[18] = {
    1, 5, 9, 13, 17, 24, 27, 34, 35, 44,
    45, 52, 53, 62, 65, 70, 71, 80
};

static seq_t *cache = NULL;          /* Cached values up to current cap   */
static size_t cache_cap = 0;         /* Allocated cache capacity          */
static idx_t  max_computed = 17;     /* Highest index cached so far       */

seq_t a(idx_t n)
{
    if (n < 18) {
        /* Return the initial values directly */
        return base_vals[n];
    }

    /* Initialise the cache if it does not exist */
    if (cache == NULL) {
        cache_cap = 18;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 18; ++i) {
            cache[i] = base_vals[i];
        }
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – abort gracefully */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms one by one up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = cache[i-1]
                  + 2*cache[i-4]
                  + 2*cache[i-7]
                  + 2*cache[i-10]
                  + cache[i-13];

        seq_t neg = cache[i-3]
                  + cache[i-5]
                  + cache[i-6]
                  + cache[i-8]
                  + cache[i-9]
                  + cache[i-11]
                  + cache[i-14];

        cache[i] = pos - neg;
    }

    max_computed = n;
    return cache[n];
}
