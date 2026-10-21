/*
 * A266769  Expansion of 1/((1-x)*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A266769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) + 2*a(n-6)
 *            + a(n-7) - a(n-8) initial values: a(0)=1, a(1)=1, a(2)=3,
 *            a(3)=4, a(4)=7, a(5)=9, a(6)=14, a(7)=17
 * Range:     u8 n <= 22, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* memoization buffer */
    static idx_t cache_cap = 0;         /* allocated capacity */
    static idx_t cache_len = 0;         /* number of sequential terms stored */

    /* Ensure the cache can hold the requested index */
    if (cache_cap <= n) {
        idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First use: initialise the eight known terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)9;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)17;
        cache_len = 8;
    }

    /* Compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i-1];
            val += (seq_t)2 * cache[i-2];
            val -= cache[i-3];
            val -= (seq_t)2 * cache[i-4];
            val -= cache[i-5];
            val += (seq_t)2 * cache[i-6];
            val += cache[i-7];
            val -= cache[i-8];
            cache[i] = val;
        }
        cache_len = n + 1;  /* now the cache contains terms 0..n */
    }

    return cache[n];
}
