/*
 * A310185  Coordination sequence Gal.4.15.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + a(n-6) - a(n-9) for n > 9 Initial values:
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=10, a(4)=13, a(5)=19, a(6)=20,
 *            a(7)=23, a(8)=29, a(9)=30 (0‑based)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  The cache stores values a[0] … a[k-1]
       where k is the number of values computed.                                            */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored in cache    */
    static size_t cache_cap = 0;   /* allocated capacity of cache        */

    /* Helper to initialise the array with the first ten terms. */
    if (cache == NULL) {
        /* Allocate a minimum of 10 slots, but cover the first requested n. */
        size_t init_cap = 10;
        if ((idx_t)init_cap < n+1) init_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* unlikely, but safe */
        cache_cap = init_cap;

        /* Set the known initial values. */
        cache[0] = 1;  cache[1] = 3;  cache[2] = 7;  cache[3] = 10;
        cache[4] = 13; cache[5] = 19; cache[6] = 20; cache[7] = 23;
        cache[8] = 29; cache[9] = 30;
        cache_len = 10;
    }

    /* If we need more values than we currently have, compute them. */
    if ((idx_t)n >= cache_len) {
        /* Ensure enough capacity */
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 10;
        while ((size_t)(n+1) > new_cap) {
            new_cap <<= 1;          /* double capacity until sufficient */
        }
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* cannot allocate further */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute new terms up to n inclusive.   */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 10) {
                /* The first ten values are already set above. */
                continue;
            } else {
                /* Recurrence: a(i) = a(i-3) + a(i-6) - a(i-9) */
                cache[i] = cache[i-3] + cache[i-6] - cache[i-9];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
