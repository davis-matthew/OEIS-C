/*
 * A313154  Coordination sequence Gal.6.252.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 9, a(3) = 14, a(4) = 20, a(5) = 26,
 *            a(6) = 32, a(7) = 38, a(8) = 43, a(9) = 48, a(10) = 52, a(n) =
 *            2*a(n‑1) - 2*a(n‑2) + 2*a(n‑3) - 2*a(n‑4) + 2*a(n‑5) - 2*a(n‑6)
 *            + 2*a(n‑7) - 2*a(n‑8) + 2*a(n‑9) - a(n‑10) for n > 10
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[11] = {
        1, 4, 9, 14, 20, 26, 32, 38, 43, 48, 52
    };
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < 11) return init[n];

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        cache_len = 11;
    }

    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-3] +
                    (seq_t)2 * cache[i-5] + (seq_t)2 * cache[i-7] +
                    (seq_t)2 * cache[i-9];
        seq_t neg = (seq_t)2 * cache[i-2] + (seq_t)2 * cache[i-4] +
                    (seq_t)2 * cache[i-6] + (seq_t)2 * cache[i-8];
        cache[i] = pos - neg - cache[i-10];
    }
    cache_len = n + 1;
    return cache[n];
}
