/*
 * A310102  Coordination sequence Gal.4.16.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-7) - a(n-14) for n > 14
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial values: a(0)=1, a(1)=3, a(2)=6, a(3)=11, a(4)=16, a(5)=18, a(6)=21,
   a(7)=24, a(8)=27, a(9)=30, a(10)=38, a(11)=43, a(12)=42, a(13)=45, a(14)=48 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    const idx_t INIT_SIZE = 15;
    static const seq_t init_vals[15] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)11, (seq_t)16,
        (seq_t)18, (seq_t)21, (seq_t)24, (seq_t)27, (seq_t)30,
        (seq_t)38, (seq_t)43, (seq_t)42, (seq_t)45, (seq_t)48
    };

    if (!cache) {
        cache = (seq_t *)malloc(INIT_SIZE * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < INIT_SIZE; ++i) cache[i] = init_vals[i];
        cache_size = INIT_SIZE;
    }

    if (n < cache_size) return cache[n];

    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        free(cache);
        exit(1);
    }
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i - 7] * (seq_t)2 - cache[i - 14];
    }
    cache_size = n + 1;
    return cache[n];
}
