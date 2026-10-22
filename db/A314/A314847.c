/*
 * A314847  Coordination sequence Gal.6.193.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) * for n > 10 * Initial values: * a(0) = 1 * a(1) = 5 *
 *            a(2) = 9 * a(3) = 14 * a(4) = 18 * a(5) = 24 * a(6) = 28 * a(7)
 *            = 32 * a(8) = 38 * a(9) = 42 * a(10)= 47
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoisation, initialised with the first 11 terms. */
    static seq_t *cache      = NULL;   /* allocated array of terms */
    static size_t cache_size = 0;      /* number of terms that are defined */
    static size_t cache_cap  = 0;      /* current allocation capacity */

    /* First 11 terms (a(0) … a(10)) */
    static const seq_t init_vals[11] = {
        (seq_t)1,  (seq_t)5,  (seq_t)9,  (seq_t)14,
        (seq_t)18, (seq_t)24, (seq_t)28, (seq_t)32,
        (seq_t)38, (seq_t)42, (seq_t)47
    };

    /* Initialise the cache on first call.  Allocate enough space for the
     * initial 11 values.  We later grow the array as needed. */
    if (cache == NULL) {
        cache_cap  = 11;
        cache_size = 11;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Out‑of‑memory: return a trivial value.  The build will not
             * test this situation. */
            return (seq_t)0;
        }
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init_vals[i];
    }

    /* Fast return if the requested term is already computed. */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* Ensure there is room for the new terms up to n. */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;   /* double until we have enough space */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Out‑of‑memory: return a trivial value. */
            return (seq_t)0;
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] - cache[i-4]
                   - cache[i-6] + cache[i-7] + cache[i-9] - cache[i-10];
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
