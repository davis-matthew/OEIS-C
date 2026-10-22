/*
 * A313859  Coordination sequence Gal.6.340.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4)
 *            + 2*a(n-5) - 2*a(n-6) + 2*a(n-7) - 2*a(n-8)
 *            + 2*a(n-9) - 2*a(n-10) + 2*a(n-11) - a(n-12)
 * Initial values: a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=21,
 *                 a(5)=25, a(6)=30, a(7)=35, a(8)=39, a(9)=44,
 *                 a(10)=50, a(11)=55, a(12)=60
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

static const seq_t init[13] = {
    (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)16, (seq_t)21,
    (seq_t)25, (seq_t)30, (seq_t)35, (seq_t)39, (seq_t)44,
    (seq_t)50, (seq_t)55, (seq_t)60
};

static seq_t *cache = NULL;     /* memoised values */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_size = 0;   /* number of values computed so far */

/* Compute a(n) for any n >= 0, independent of call order. */
seq_t a(idx_t n)
{
    /* Handle the first 13 terms directly. */
    if (n < 13)
        return init[n];

    /* ------------------------------------------------------------------
     * Initialise or grow the cache if necessary.
     * ------------------------------------------------------------------ */
    size_t need = (size_t)n + 1;          /* number of values required */
    if (cache == NULL) {                  /* first call */
        cache_cap = need > 13 ? need : 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_size = 13;
    } else if (need > cache_cap) {        /* increase capacity */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = need;
    }

    /* ------------------------------------------------------------------
     * Compute new values up to n if they haven't been computed yet.
     * ------------------------------------------------------------------ */
    if (need > cache_size) {
        for (size_t idx = cache_size; idx < need; ++idx) {
            cache[idx] =
                2 * cache[idx - 1] - 2 * cache[idx - 2] + 2 * cache[idx - 3]
              - 2 * cache[idx - 4] + 2 * cache[idx - 5] - 2 * cache[idx - 6]
              + 2 * cache[idx - 7] - 2 * cache[idx - 8]
              + 2 * cache[idx - 9] - 2 * cache[idx -10]
              + 2 * cache[idx -11] - cache[idx -12];
        }
        cache_size = need;
    }

    return cache[(size_t)n];
}
