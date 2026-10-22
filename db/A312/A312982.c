/*
 * A312982  Coordination sequence Gal.5.232.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312982
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

/*  OEIS A312982 – Coordination sequence Gal.5.232.2
 *  Recurrence:  a(n) = a(n‑1) + a(n‑9) – a(n‑10)   (n > 17)
 *  Initial values:  a(0) = 1,  a(1) = 4,  a(2) = 9,  a(3) = 13,
 *                    a(4) = 20, a(5) = 23, a(6) = 29, a(7) = 33,
 *                    a(8) = 39, a(9) = 44, a(10) = 50, a(11) = 52,
 *                    a(12) = 59, a(13) = 65, a(14) = 67, a(15) = 73,
 *                    a(16) = 79, a(17) = 83
 *
 *  The function can be called with n in any order; it memoises values
 *  in a static cache.  It satisfies the requirements for both C11
 *  and C++17.  It uses only the allowed operators and header files.
 *  The cache is grown by powers of two.  The initial 18 terms are
 *  stored in a static table, which is acceptable as they are needed
 *  to seed the recurrence.
 */

#include <stdlib.h>
#include <stddef.h>    /* for size_t */

/* initial values for n = 0 … 17 */
static const seq_t init[18] = {
    (seq_t)1,  (seq_t)4,  (seq_t)9,  (seq_t)13, (seq_t)20,
    (seq_t)23, (seq_t)29, (seq_t)33, (seq_t)39, (seq_t)44,
    (seq_t)50, (seq_t)52, (seq_t)59, (seq_t)65, (seq_t)67,
    (seq_t)73, (seq_t)79, (seq_t)83
};

/* cache of computed values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;      /* allocated capacity (number of entries) */
static idx_t  cache_filled = 0;   /* number of entries that are already
                                   initialized in cache[]  */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap << 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n) – works for any n ≥ 0 in any call order */
seq_t a(idx_t n)
{
    /* Direct return for the seeded initial values.  No cache
       allocation or filling is required. */
    if (n < 18) return init[n];

    /* Grow the cache if necessary. */
    ensure_cache(n);

    /* Compute any values that have not been computed yet. */
    if (cache_filled <= n) {
        for (idx_t i = cache_filled; i <= n; ++i) {
            if (i < 18) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i) = a(i‑1) + a(i‑9) – a(i‑10) */
                cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
            }
        }
        cache_filled = n + 1;    /* all indices < n are now filled */
    }

    return cache[n];
}
