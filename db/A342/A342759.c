/*
 * A342759  Fold a square sheet of paper alternately vertically to the left
 *          and horizontally downwards; after each fold, draw a line along
 *          each inward crease; after n folds, the resulting graph has a(n)
 *          regions.
 * https://oeis.org/A342759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=10, a(6)=16,
 *            a(7)=25. For n ≥ 8: if n is even: a(n) = 3·a(n‑1) – 2·a(n‑2) if
 *            n is odd : a(n) = 3·a(n‑1) – 2·a(n‑2) – 3·2^((n‑7)/2)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* base values for 0 ≤ n ≤ 7 */
    static const seq_t base[8] = {1, 2, 3, 4, 6, 10, 16, 25};

    if (n < 8) return base[n];

    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialize cache the first time */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        for (idx_t i = 0; i < 8; ++i) cache[i] = base[i];
        cache_size = 8;
    }

    /* extend cache if necessary */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed */
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if ((i & 1) == 0) {   /* even index */
                cache[i] = (seq_t)3 * cache[i - 1] - (seq_t)2 * cache[i - 2];
            } else {              /* odd index (≥9) */
                seq_t sub = (seq_t)3 * pow2((i - 7) / 2);
                cache[i] = (seq_t)3 * cache[i - 1] - (seq_t)2 * cache[i - 2] - sub;
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
