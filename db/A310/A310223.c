/*
 * A310223  Coordination sequence Gal.6.12.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 7, a(3) = 12, a(4) = 15, a(5) = 18,
 *            a(6) = 21, a(7) = 28, a(8) = 33, a(9) = 33, a(10) = 36, a(11) =
 *            39, a(12) = 47, a(13) = 52, a(14) = 51. For n ≥ 15: a(n) =
 *            a(n-1) - a(n-2) + a(n-3) - a(n-4) + 2*a(n-5) - 2*a(n-6) +
 *            2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10) + a(n-11) -...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t sz = 0;

    /* initialise the first 15 values on first call */
    if (sz == 0) {
        cache = (seq_t *)malloc(15 * sizeof(seq_t));
        sz    = 15;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)18;
        cache[6] = (seq_t)21;
        cache[7] = (seq_t)28;
        cache[8] = (seq_t)33;
        cache[9] = (seq_t)33;
        cache[10] = (seq_t)36;
        cache[11] = (seq_t)39;
        cache[12] = (seq_t)47;
        cache[13] = (seq_t)52;
        cache[14] = (seq_t)51;
    }

    /* return if already computed */
    if ((size_t)n < sz) {
        return cache[n];
    }

    /* extend the cache to size n+1 */
    size_t new_sz = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!new_cache) {          /* allocation failed – return a safe value */
        return (seq_t)0;
    }
    cache = new_cache;

    /* compute missing terms */
    for (size_t i = sz; i < new_sz; ++i) {
        seq_t val =
            cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4]
          + (seq_t)2 * cache[i-5] - (seq_t)2 * cache[i-6]
          + (seq_t)2 * cache[i-7] - (seq_t)2 * cache[i-8]
          + (seq_t)2 * cache[i-9] - cache[i-10] + cache[i-11]
          - cache[i-12] + cache[i-13] - cache[i-14];
        cache[i] = val;
    }

    sz = new_sz;
    return cache[n];
}
