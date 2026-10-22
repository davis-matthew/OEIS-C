/*
 * A310525  Coordination sequence Gal.4.74.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            a(n-6) for n > 6 Initial terms: a(0)=1, a(1)=4, a(2)=10,
 *            a(3)=16, a(4)=22, a(5)=28, a(6)=32
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure cache has at least n+1 entries */
    if (cache_len == 0) {
        /* Allocate space for first 7 terms */
        cache = (seq_t *)calloc(7, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, return 0 as fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)28;
        cache[6] = (seq_t)32;
        cache_len = 7;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* Extend cache to n+1 entries */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0; /* allocation failure */
    cache = new_cache;

    for (idx_t i = cache_len; i <= n; i++) {
        if (i <= 6) {
            /* initial values already set when cache_len was 7, but keep for safety */
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)4; break;
                case 2: cache[i] = (seq_t)10; break;
                case 3: cache[i] = (seq_t)16; break;
                case 4: cache[i] = (seq_t)22; break;
                case 5: cache[i] = (seq_t)28; break;
                case 6: cache[i] = (seq_t)32; break;
            }
        } else {
            cache[i] =  (seq_t)2 * cache[i - 1]
                       - (seq_t)2 * cache[i - 2]
                       + (seq_t)2 * cache[i - 3]
                       - (seq_t)2 * cache[i - 4]
                       + (seq_t)2 * cache[i - 5]
                       -       cache[i - 6];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
