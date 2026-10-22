/*
 * A311491  Coordination sequence Gal.5.91.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - a(n-6)
 *            + a(n-7) - a(n-8) for n > 18, with initial values: a(0)=1,
 *            a(1)=4, a(2)=8, a(3)=12, a(4)=17, a(5)=22, a(6)=25, a(7)=31,
 *            a(8)=35, a(9)=39, a(10)=45, a(11)=49, a(12)=52, a(13)=59,
 *            a(14)=62, a(15)=65, a(16)=73, a(17)=76, a(18)=78
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;   /* index of last computed term */

    /* initial values for n = 0..18 */
    static const seq_t init_vals[19] = {
        (seq_t)1,  (seq_t)4,  (seq_t)8,  (seq_t)12, (seq_t)17,
        (seq_t)22, (seq_t)25, (seq_t)31, (seq_t)35, (seq_t)39,
        (seq_t)45, (seq_t)49, (seq_t)52, (seq_t)59, (seq_t)62,
        (seq_t)65, (seq_t)73, (seq_t)76, (seq_t)78
    };

    /* initialise cache on first call */
    if (!cache)
    {
        /* start with a small capacity, ensure it holds the first 19 terms */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);          /* abort if memory allocation fails */

        /* store the known initial terms */
        for (idx_t i = 0; i < 19; ++i)
            cache[i] = init_vals[i];

        cache_last = 18;              /* last index computed so far */
    }

    /* if the requested term is already computed, return it */
    if (n <= cache_last)
        return cache[n];

    /* extend the cache to accommodate up to n */
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;           /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* abort if reallocation fails */

        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms in order */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        /* recurrence uses the 8 previous terms */
        int64_t val = (int64_t)cache[i - 1];
        val -= (int64_t)cache[i - 2];
        val += 2LL * (int64_t)cache[i - 3];
        val -= 2LL * (int64_t)cache[i - 4];
        val += 2LL * (int64_t)cache[i - 5];
        val -= (int64_t)cache[i - 6];
        val += (int64_t)cache[i - 7];
        val -= (int64_t)cache[i - 8];

        cache[i] = (seq_t)val;   /* cast back to seq_t */
    }

    cache_last = n;
    return cache[n];
}
