/*
 * A315540  Coordination sequence Gal.6.643.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A315540: Coordination sequence Gal.6.643.4
    Recurrence:
      a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - 2*a(n-6)
             + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - 2*a(n-10) + 2*a(n-11) - a(n-12)
      a(0) = 1, a(1) = 6, a(2) = 11, a(3) = 17, a(4) = 23, a(5) = 29,
      a(6) = 34, a(7) = 39, a(8) = 45, a(9) = 51, a(10) = 57, a(11) = 62,
      a(12) = 68
*/

#include <stdlib.h>

/* static cache storage */
static seq_t *cache = NULL;           /* cached values */
static idx_t cache_size = 0;          /* allocated size of cache */
static idx_t computed_max = (idx_t)-1;/* highest index that has been computed */

/* precomputed first 13 values */
static const seq_t init_vals[13] = {
    (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)17, (seq_t)23, (seq_t)29,
    (seq_t)34, (seq_t)39, (seq_t)45, (seq_t)51, (seq_t)57, (seq_t)62,
    (seq_t)68
};

seq_t a(idx_t n)
{
    /* Allocate cache if not yet allocated */
    if (!cache) {
        idx_t cap = n + 1;
        if (cap < 13) cap = 13;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        for (idx_t i = 0; i < 13; ++i) cache[i] = init_vals[i];
        cache_size   = cap;
        computed_max = 12;            /* indices 0..12 are known */
    }

    /* Grow cache if requested index is outside current bounds */
    if (n >= cache_size) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* Initialize new part to zero */
        for (idx_t i = cache_size; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_cap;
    }

    /* Return known value if n is within precomputed range */
    if (n <= 12) return cache[n];

    /* Compute missing terms up to n */
    if (computed_max < n) {
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            seq_t val = (seq_t)0;
            /* Sum over offsets 1..11 with alternating sign and factor 2 */
            for (idx_t offset = 1; offset <= 11; ++offset) {
                seq_t term = cache[k - offset];
                if (offset & 1) {             /* odd offset: +2*term */
                    val += term * (seq_t)2;
                } else {                      /* even offset: -2*term */
                    val -= term * (seq_t)2;
                }
            }
            val -= cache[k - 12];
            cache[k] = val;
        }
        computed_max = n;
    }

    return cache[n];
}
