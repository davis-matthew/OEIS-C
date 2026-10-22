/*
 * A315688  Coordination sequence Gal.6.343.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A315688: A coordination sequence for Gal.6.343.5
    Recurrence (n > 10):
        a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) - a(n-10)
    Initial values: a(0)…a(10) = {1,6,12,17,22,28,32,36,42,47,52}
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to hold computed values.  It grows as needed. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* current capacity (number of elements) */
    static idx_t last_computed = (idx_t)(-1); /* max index for which cache is valid */

    /* Pre‑computed values for n = 0 … 10.  Use an array for quick access. */
    static const seq_t init[11] = {
        1, 6, 12, 17, 22, 28, 32, 36, 42, 47, 52
    };

    /* Direct answer for the first 11 terms – no caching needed. */
    if (n <= 10) {
        return init[(size_t)n];
    }

    /* Ensure the cache can hold indices up to n (size n+1 elements). */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap <= (size_t)n + 1) {
            newcap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort (should not happen for typical limits). */
            abort();
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise cache with the first 11 terms on first use. */
    if (last_computed == (idx_t)(-1)) {
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
        last_computed = 10;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t pos = cache[i-1] + cache[i-3] + cache[i-7] + cache[i-9];
        seq_t neg = cache[i-4] + cache[i-6] + cache[i-10];
        cache[i]  = pos - neg;          /* pos ≥ neg for this sequence */
    }
    last_computed = n;
    return cache[n];
}
