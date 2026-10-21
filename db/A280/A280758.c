/*
 * A280758  Expansion of (1 + x + x^2)/(1 - x - 3*x^2 - x^3 + x^4).
 * https://oeis.org/A280758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3) - a(n-4) for n ≥ 4 with
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=13
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Quick return for the first four terms */
    if (n < 4) {
        static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)13 };
        return init[n];
    }

    /* Static cache for memoization */
    static seq_t *cache = NULL;     /* dynamic array holding a(0) … a(k) */
    static idx_t  cache_len = 0;    /* allocated length of cache */
    static idx_t  computed = 0;     /* next index to compute (i.e. first
                                      index *not* yet computed) */

    /* Allocate or grow the cache to hold up to n */
    if (cache == NULL) {
        cache_len = (n >= 4) ? (n + 1) : 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)13;
        computed = 4;           /* up to index 3 inclusive are ready */
    } else if (n + 1 > cache_len) {
        /* grow cache by doubling until it can hold up to n */
        idx_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* out of memory */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* a(i) = a(i-1) + 3*a(i-2) + a(i-3) - a(i-4) */
        seq_t x = cache[i-1];
        seq_t y = ((seq_t)3) * cache[i-2];
        seq_t z = cache[i-3];
        seq_t w = cache[i-4];
        cache[i] = x + y + z - w;
    }
    computed = n + 1;

    return cache[n];
}
