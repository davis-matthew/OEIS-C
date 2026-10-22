/*
 * A313685  Coordination sequence Gal.4.43.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) – 2*a(n-2) + 2*a(n-3) – 2*a(n-4) + 2*a(n-5) –
 *            2*a(n-6) + 2*a(n-7) – a(n-8) for n>8 Initial values: a(0)=1,
 *            a(1)=5, a(2)=10, a(3)=15, a(4)=19, a(5)=23, a(6)=28, a(7)=33,
 *            a(8)=38
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static const seq_t init_vals[9] = {1, 5, 10, 15, 19, 23, 28, 33, 38};

seq_t a(idx_t n)
{
    while (cache_len <= n) {
        seq_t val;
        idx_t i = cache_len;

        if (i < 9) {
            val = init_vals[i];
        } else {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            seq_t v5 = cache[i-5];
            seq_t v6 = cache[i-6];
            seq_t v7 = cache[i-7];
            seq_t v8 = cache[i-8];

            val = 2*v1 - 2*v2 + 2*v3 - 2*v4 + 2*v5 - 2*v6 + 2*v7 - v8;
        }

        /* Grow the cache to hold the new term. */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[cache_len + 1];
        if (cache) {
            for (idx_t j = 0; j < cache_len; ++j) new_cache[j] = cache[j];
            delete[] cache;
        }
#else
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(cache_len + 1) * sizeof(seq_t));
        if (!new_cache) abort();
#endif
        cache = new_cache;
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[(size_t)n];
}
