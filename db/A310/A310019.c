/*
 * A310019  Coordination sequence Gal.6.110.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310019
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

/*  A310019: Coordination sequence Gal.6.110.1
    Recurrence (Conjecture):
        a(n) =  a(n-1) - 2a(n-2) + 2a(n-3) - 2a(n-4) + 2a(n-5)
               - a(n-6) + 2a(n-7) - a(n-8) + 2a(n-9) - 2a(n-10)
               + 2a(n-11) - 2a(n-12) + a(n-13) - a(n-14)      for n >= 17
    Initial values (used for n <= 16):
      a(0)=1,  a(1)=3,  a(2)=5,  a(3)=10,  a(4)=17,  a(5)=21,
      a(6)=20, a(7)=19, a(8)=23, a(9)=34, a(10)=45, a(11)=45,
      a(12)=38, a(13)=39, a(14)=45, a(15)=52, a(16)=63
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static bool initialized = false;

    /* initial values up to n = 16 */
    static const seq_t init[17] = {
        1, 3, 5, 10, 17, 21, 20, 19, 23, 34,
        45, 45, 38, 39, 45, 52, 63
    };

    if (!initialized) {
        cache_size = 17;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) { /* allocation failed, return 0 as fallback */
            return (seq_t)0;
        }
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
        initialized = true;
    }

    /* if requested n is within pre‑computed range */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* grow cache if necessary */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; cannot compute further */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (size_t i = 17; i <= (size_t)n; ++i) {
        /* compute positive part */
        seq_t pos = cache[i-1];
        pos += cache[i-3] << 1;
        pos += cache[i-5] << 1;
        pos += cache[i-7] << 1;
        pos += cache[i-9] << 1;
        pos += cache[i-11] << 1;
        pos += cache[i-13];

        /* compute negative part */
        seq_t neg = (cache[i-2] << 1);
        neg += (cache[i-4] << 1);
        neg += (cache[i-10] << 1);
        neg += (cache[i-12] << 1);
        neg += cache[i-6];
        neg += cache[i-8];
        neg += cache[i-14];

        cache[i] = pos - neg;
    }

    return cache[(size_t)n];
}
