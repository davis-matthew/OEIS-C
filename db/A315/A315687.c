/*
 * A315687  Coordination sequence Gal.6.210.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) +
 *            3*a(n-7) - 4*a(n-8) + 3*a(n-9) - a(n-10) for n > 10 Initial
 *            values: a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=22, a(5)=27,
 *            a(6)=33, a(7)=39, a(8)=44, a(9)=49, a(10)=54
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for previous terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = (idx_t)-1;   /* max index computed */

    /* initialize base values at first call */
    if (cache == NULL) {
        cache_cap = 11;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure: return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)27;
        cache[6] = (seq_t)33;
        cache[7] = (seq_t)39;
        cache[8] = (seq_t)44;
        cache[9] = (seq_t)49;
        cache[10] = (seq_t)54;
        cache_max = 10;
    }

    /* if we already have the term, return it */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* extend cache array if needed */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = 0;
        /* recurrence: a(i) = 3*a(i-1) - 4*a(i-2) + 3*a(i-3) - a(i-4)
                         - a(i-6) + 3*a(i-7) - 4*a(i-8) + 3*a(i-9) - a(i-10) */
        val += (seq_t)3 * cache[(size_t)(i - 1)];
        val -= (seq_t)4 * cache[(size_t)(i - 2)];
        val += (seq_t)3 * cache[(size_t)(i - 3)];
        val -= cache[(size_t)(i - 4)];
        val -= cache[(size_t)(i - 6)];
        val += (seq_t)3 * cache[(size_t)(i - 7)];
        val -= (seq_t)4 * cache[(size_t)(i - 8)];
        val += (seq_t)3 * cache[(size_t)(i - 9)];
        val -= cache[(size_t)(i - 10)];
        cache[(size_t)i] = val;
    }
    cache_max = n;
    return cache[(size_t)n];
}
