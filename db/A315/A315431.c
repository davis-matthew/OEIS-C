/*
 * A315431  Coordination sequence Gal.6.669.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A315431 – Coordination sequence Gal.6.669.6
 *
 * Recurrence (valid for n > 22):
 *   a(n) = a(n‑4) – a(n‑5) + a(n‑7) + a(n‑9) – a(n‑11) + a(n‑12) – a(n‑16)
 *
 * Initial values (n = 0 … 23):
 *   1, 6, 11, 15, 22, 28, 33, 39, 44, 52,
 *   56, 61, 64, 74, 78, 83, 88, 98, 100, 105,
 *   110, 120, 120, 129
 *
 * The function keeps a cache of all values up to the largest n
 * requested so far, enabling O(n) time for each new maximum n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial 24 terms of the sequence */
    static const seq_t init[24] = {
        (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)15, (seq_t)22, (seq_t)28,
        (seq_t)33, (seq_t)39, (seq_t)44, (seq_t)52,
        (seq_t)56, (seq_t)61, (seq_t)64, (seq_t)74, (seq_t)78, (seq_t)83,
        (seq_t)88, (seq_t)98, (seq_t)100, (seq_t)105,
        (seq_t)110, (seq_t)120, (seq_t)120, (seq_t)129
    };

    /* Cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* If we haven't computed up to n yet, grow the cache */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!new_cache) return 0;       /* allocation failed – return 0 */
        cache = new_cache;
        /* Compute new elements */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i < 24) {
                cache[i] = init[i];
            } else {
                cache[i] =
                    cache[i-4] - cache[i-5] + cache[i-7] + cache[i-9]
                    - cache[i-11] + cache[i-12] - cache[i-16];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
