/*
 * A315732  Coordination sequence Gal.6.640.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) for n>10;
 *            initial values: a(0)=1, a(1)=6, a(2)=12, a(3)=18, a(4)=22,
 *            a(5)=28, a(6)=34, a(7)=38, a(8)=44, a(9)=50, a(10)=56
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute A315732(n) */
seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)6, (seq_t)12, (seq_t)18, (seq_t)22,
        (seq_t)28, (seq_t)34, (seq_t)38, (seq_t)44, (seq_t)50,
        (seq_t)56
    };

    /* static cache */
    static seq_t *cache = NULL;     /* array of computed values */
    static idx_t cache_len = 0;     /* allocated size (elements) */
    static idx_t cache_max = 0;     /* maximum index computed so far */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_len = 11;  /* store up to index 10 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed: return 0 */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        cache_max = 10;
    }

    /* if requested index already cached, return promptly */
    if (n < cache_len) {
        return cache[n];
    }

    /* ensure cache large enough */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* reallocation failed; keep old cache and return 0 */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* compute using the given recurrence with only positive arithmetic */
        seq_t pos = 2*cache[i-1] + 2*cache[i-3] + 2*cache[i-5] + 2*cache[i-7] + 2*cache[i-9];
        seq_t neg = 2*cache[i-2] + 2*cache[i-4] + 2*cache[i-6] + 2*cache[i-8] + cache[i-10];
        cache[i] = pos - neg;
    }
    cache_max = n;
    return cache[n];
}
