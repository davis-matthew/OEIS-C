/*
 * A314839  Coordination sequence Gal.6.357.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) - a(n-3) - a(n-4) + a(n-5) - a(n-7) + 2*a(n-8) +
 *            a(n-9) - 2*a(n-10) + a(n-11) + 2*a(n-12) - a(n-13) + a(n-15) -
 *            a(n-16) - a(n-17) + a(n-18) - a(n-20) Initial values: a(0)=1,
 *            a(1)=5, a(2)=9, a(3)=14, a(4)=18, a(5)=23, a(6)=27, a(7)=33,
 *            a(8)=38, a(9)=43, a(10)=47, a(11)=49, a(12)=56,...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored (indices 0..cache_len-1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initial values up to n = 20 */
    static const seq_t init[] = {
        1,5,9,14,18,23,27,33,38,43,
        47,49,56,58,65,69,76,81,85,86,92
    };
    const size_t init_len = sizeof(init)/sizeof(init[0]);

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = init_len;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        for (size_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        cache_len = init_len;
    }

    /* if requested term already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* ensure enough capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 20)
            /* values 0..20 are already stored in init */
            continue;
        long long val = 0;
        val += (long long)cache[i-2];
        val -= (long long)cache[i-3];
        val -= (long long)cache[i-4];
        val += (long long)cache[i-5];
        val -= (long long)cache[i-7];
        val += 2LL * (long long)cache[i-8];
        val += (long long)cache[i-9];
        val -= 2LL * (long long)cache[i-10];
        val += (long long)cache[i-11];
        val += 2LL * (long long)cache[i-12];
        val -= (long long)cache[i-13];
        val += (long long)cache[i-15];
        val -= (long long)cache[i-16];
        val -= (long long)cache[i-17];
        val += (long long)cache[i-18];
        val -= (long long)cache[i-20];
        cache[i] = (seq_t)val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
