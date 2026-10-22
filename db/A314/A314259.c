/*
 * A314259  Coordination sequence Gal.3.60.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) * -
 *            3*a(n-6) + 4*a(n-7) - 3*a(n-8) + 2*a(n-9) - 2*a(n-10) * +
 *            2*a(n-11) - 2*a(n-12) + 2*a(n-13) - a(n-14) for n > 14 * with
 *            initial values: * a(0)=1, a(1)=5, a(2)=11, a(3)=18, a(4)=22,
 *            a(5)=28, a(6)=34, * a(7)=40, a(8)=45, a(9)=49, a(10)...
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[15] = {
    (seq_t)1,  (seq_t)5,  (seq_t)11, (seq_t)18, (seq_t)22,
    (seq_t)28, (seq_t)34, (seq_t)40, (seq_t)45, (seq_t)49,
    (seq_t)57, (seq_t)62, (seq_t)67, (seq_t)73, (seq_t)79
};

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* memory allocated (elements) */
    static size_t cache_len = 0;     /* elements already computed */

    /* return immediately for the first 15 terms */
    if (n <= 14) return init_vals[n];

    /* create cache if needed */
    if (cache == NULL) {
        cache_cap = 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – unlikely in tests */
        for (size_t i = 0; i < 15; ++i) cache[i] = init_vals[i];
        cache_len = 15;
    }

    /* grow cache if the requested index exceeds current capacity */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n (if not already computed) */
    if ((size_t)n + 1 > cache_len) {
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            int64_t t = 0;
            t +=  2 * (int64_t)cache[i-1];
            t -=  2 * (int64_t)cache[i-2];
            t +=  2 * (int64_t)cache[i-3];
            t -=  2 * (int64_t)cache[i-4];
            t +=  2 * (int64_t)cache[i-5];
            t -=  3 * (int64_t)cache[i-6];
            t +=  4 * (int64_t)cache[i-7];
            t -=  3 * (int64_t)cache[i-8];
            t +=  2 * (int64_t)cache[i-9];
            t -=  2 * (int64_t)cache[i-10];
            t +=  2 * (int64_t)cache[i-11];
            t -=  2 * (int64_t)cache[i-12];
            t +=  2 * (int64_t)cache[i-13];
            t -=  1 * (int64_t)cache[i-14];
            cache[i] = (seq_t)t;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
