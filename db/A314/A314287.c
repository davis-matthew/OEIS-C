/*
 * A314287  Coordination sequence Gal.6.654.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-9) - a(n-10) + a(n-11) -
 *            a(n-12) for n > 12. Initial values (n=0..12): 1, 5, 12, 17, 24,
 *            27, 32, 37, 46, 51, 54, 61, 68
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of elements stored */

    /* hard‑coded initial values */
    static const seq_t base[13] = {
        1ULL, 5ULL, 12ULL, 17ULL, 24ULL, 27ULL, 32ULL, 37ULL, 46ULL,
        51ULL, 54ULL, 61ULL, 68ULL
    };

    /* answer lies in the base array for n < 13 */
    if (n < 13)
        return base[(size_t)n];

    /* desired cache size = n+1 elements (indices 0..n) */
    size_t need = (size_t)n + 1;

    /* Allocate cache if it doesn't yet exist */
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (!cache) abort();               /* memory allocation failure */
        cache_size = 0;
    }

    /* Grow the cache if necessary */
    if (need > cache_size) {
        /* Preserve the old size before growing */
        size_t old_size = cache_size;

        /* Realloc to new larger size */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* initialize newly allocated part with base values if still missing */
        if (old_size < 13) {
            size_t up = 13 < need ? 13 : need;
            for (size_t i = old_size; i < up; ++i)
                cache[i] = base[i];
            old_size = up;
        }

        /* Compute new terms from old_size up to n (inclusive) */
        for (size_t i = old_size; i <= n; ++i) {
            seq_t t = cache[i-1] - cache[i-2];
            t += cache[i-3];
            t += cache[i-9];
            t -= cache[i-10];
            t += cache[i-11];
            t -= cache[i-12];
            cache[i] = t;
        }

        cache_size = need;
    }

    /* For n < cache_size the value is already computed */
    return cache[(size_t)n];
}
