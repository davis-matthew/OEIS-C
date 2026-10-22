/*
 * A311088  Coordination sequence Gal.4.25.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311088
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

/* Sequence A311088 (Gal.4.25.4): 
   a(n) = a(n-1) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-9) - a(n-10)  (n > 10)
   initial values: a(0)=1, a(1)=4, a(2)=8, a(3)=12, a(4)=15, a(5)=17,
   a(6)=20, a(7)=24, a(8)=29, a(9)=34, a(10)=37 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;          /* array of a(0)...a(cacheSize-1) */
    static idx_t cacheSize = 0;          /* number of terms stored */
    static idx_t cacheCap  = 0;          /* allocated capacity */

    /* initial values for indices 0..10 */
    static const seq_t base[11] = {
        (seq_t)1, (seq_t)4, (seq_t)8, (seq_t)12, (seq_t)15,
        (seq_t)17, (seq_t)20, (seq_t)24, (seq_t)29, (seq_t)34, (seq_t)37
    };

    /* initialise the cache on first call */
    if (cache == NULL) {
        cacheCap = 16;                           /* start with some capacity */
        cache = (seq_t*)malloc((size_t)cacheCap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* allocation failed */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = base[i];
        cacheSize = 11;   /* next not yet computed index is 11 */
    }

    /* If already computed, return it directly */
    if (n < cacheSize)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cacheCap) {
        idx_t newCap = cacheCap * 2;
        if (newCap <= n + 1) newCap = n + 1;
        seq_t *newCache = (seq_t*)realloc(cache, (size_t)newCap * sizeof(seq_t));
        if (newCache == NULL) return (seq_t)0;   /* allocation failed */
        cache = newCache;
        cacheCap = newCap;
    }

    /* compute terms from cacheSize up to n (inclusive) */
    idx_t i;
    for (i = cacheSize; i <= n; ++i) {
        /* for i <= 10 we already have base values, but loop starts at 11 */
        seq_t val = cache[i-1];
        val -= cache[i-4];
        val += cache[i-5] << 1;   /* 2 * a(i-5) */
        val -= cache[i-6];
        val += cache[i-9];
        val -= cache[i-10];
        cache[i] = val;
    }
    cacheSize = n + 1;    /* update next uncomputed index */

    return cache[n];
}
