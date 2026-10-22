/*
 * A315685  Coordination sequence Gal.4.140.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) * a(0)=1,
 *            a(1)=6, a(2)=12, a(3)=17, a(4)=22, a(5)=27, a(6)=32 (valid for
 *            n≥7)
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 6;
        cache[2] = 12;
        cache[3] = 17;
        cache[4] = 22;
        cache[5] = 27;
        cache[6] = 32;
        cache_len = 7;
    }

    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 2 * cache[i-1] - cache[i-2] - cache[i-4] + 2 * cache[i-5] - cache[i-6];
        }
        cache_len = new_len;
    }

    return cache[n];
}
