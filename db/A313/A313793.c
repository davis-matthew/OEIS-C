/*
 * A313793  Coordination sequence Gal.4.134.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) 
 *            + 2*a(n-5) - 2*a(n-6) + 2*a(n-7) - a(n-8)
 * Initial values: a(0)=1, a(1)=5, a(2)=10, a(3)=15, a(4)=21,
 *                 a(5)=27, a(6)=32, a(7)=37, a(8)=42
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size */
    static idx_t last_computed = (idx_t)-1;  /* last index computed */

    /* initial values */
    const seq_t init_vals[9] = {
        (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)15,
        (seq_t)21, (seq_t)27, (seq_t)32, (seq_t)37, (seq_t)42
    };

    /* one-time initialization */
    if (cache == NULL) {
        cache_size = 9;                       /* initial capacity 0..8 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        last_computed = 8;
    }

    /* if requested n is already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* ensure the cache is large enough */
    if (n + 1 > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < (size_t)(n + 1))
            new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {          /* allocation failure: abort */
            abort();
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* positive contributions : 2*a[i-1] + 2*a[i-3] + 2*a[i-5] + 2*a[i-7] */
        seq_t pos = (cache[i-1] << 1) + (cache[i-3] << 1)
                  + (cache[i-5] << 1) + (cache[i-7] << 1);

        /* negative contributions : 2*a[i-2] + 2*a[i-4] + 2*a[i-6] + a[i-8] */
        seq_t neg = (cache[i-2] << 1) + (cache[i-4] << 1)
                  + (cache[i-6] << 1) + cache[i-8];

        cache[i] = pos - neg;        /* subtraction is safe: pos >= neg */
    }

    last_computed = n;
    return cache[n];
}
